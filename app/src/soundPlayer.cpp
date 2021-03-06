#include "soundPlayer.h"
#include "ui_mainwindow.h"
#include "playerqss.h"
#include "mainwindow.h"
#include <QMediaMetaData>
#include <QWidget>
#include <QFileInfo>

SoundPlayer::SoundPlayer(Ui::MainWindow *child, MainWindow *parent)
{
    ui = child;
    m_parent = parent;
    m_player = new QMediaPlayer();
    m_playlist = SoundPlayer::playlist();
    m_playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    m_player->setPlaylist(m_playlist);
    connect(m_player, &QMediaPlayer::positionChanged, this, &SoundPlayer::setPosition);
    connect(m_player, &QMediaPlayer::volumeChanged, this, &SoundPlayer::setVolume);
    connect(m_player, &QMediaPlayer::metaDataAvailableChanged, this, &SoundPlayer::metaData);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &SoundPlayer::autoNext);
    ui->labelSong->setText("");
    ui->labelArtist->setText("");
    m_mode = QMediaPlaylist::PlaybackMode::Loop;
}

SoundPlayer::~SoundPlayer()
{
    delete m_player;
}

void SoundPlayer::autoNext(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::MediaStatus::EndOfMedia)
        next();
}

void SoundPlayer::setSound(QString)
{
    if (!isPlaylistExist)
        return;
//    m_player->setMedia(QUrl::fromLocalFile(path));
    if (m_player->isMetaDataAvailable())
        ui->statusbar->showMessage(m_player->currentMedia().playlist()->objectName(), 2000);
    else {
        while (!m_player->isMetaDataAvailable()) {
            QCoreApplication::processEvents();
        }
    }
    QStringList list = m_player->availableMetaData();

    this->setPlay();
    ui->playButton->click();
}

void SoundPlayer::setSound(int index, QSqlQueryModel *model) {
    if (!isPlaylistExist)
        return;
    if (model)
        setPlaylist(model);
    m_list.setStartSong(index);
//    QStringList list = m_player->availableMetaData();
    ui->playButton->click();
}

void SoundPlayer::changeMode()
{
    if (m_mode == QMediaPlaylist::PlaybackMode::Loop)
        m_mode = QMediaPlaylist::PlaybackMode::Random;
    else if (m_mode == QMediaPlaylist::PlaybackMode::Random)
        m_mode = QMediaPlaylist::PlaybackMode::CurrentItemInLoop;
    else if (m_mode == QMediaPlaylist::PlaybackMode::CurrentItemInLoop)
        m_mode = QMediaPlaylist::PlaybackMode::Loop;
    m_list.setPlaybackMode(m_mode);
}

void SoundPlayer::setPlay()
{
    QWidget tmp;

    metaData(m_player->isMetaDataAvailable());
    if (m_player->state() == QMediaPlayer::State::PausedState || m_player->state() == QMediaPlayer::State::StoppedState) {
        m_player->play();
//        ui->playButton->setIcon(tmp.style()->standardIcon(QStyle::SP_MediaPause));
        ui->playButton->setStyleSheet(PlayStyle());
        m_parent->m_systemTray->contextMenu()->actions()[1]->setIcon(QIcon(":/image/image/image/play.png"));
    }
    else if(m_player->state() == QMediaPlayer::State::PlayingState) {
        m_player->pause();
//        ui->playButton->setIcon(tmp.style()->standardIcon(QStyle::SP_MediaPlay));
        ui->playButton->setStyleSheet(PauseStyle());
        m_parent->m_systemTray->contextMenu()->actions()[1]->setIcon(QIcon(":/image/image/image/pase.png"));
    }
    ui->statusPlay->setEnabled(true);
}

void SoundPlayer::setPause()
{
    m_player->pause();
}

void SoundPlayer::setStop()
{
    m_player->stop();
}

void SoundPlayer::setVolume(int position)
{
    ui->statusVolume->setSliderPosition(position);
    m_player->setVolume(position);
}

void SoundPlayer::setPosition(int position)
{
    if (ui->statusPlay->maximum() != m_player->duration())
       ui->statusPlay->setMaximum(static_cast<int>(m_player->duration()));
    ui->statusPlay->setValue(position);

    int seconds = (position / 1000) % 60;
    int minute = (position / 60000) % 60;
    int hours = (position / 3600000) % 24;
    QTime time{hours, minute, seconds};
    ui->labelTime->setText(time.toString());
}

void SoundPlayer::fastForward() {
    int pos = m_player->position();
    pos += 10 * 1000;
    m_player->setPosition(pos);
}

void SoundPlayer::rewind()
{
    int pos = m_player->position();
    pos += -10 * 1000;
    m_player->setPosition(pos);
}
void SoundPlayer::setMovedPosition(int position)
{
    m_player->setPosition(position);
}

QMediaPlaylist *SoundPlayer::m_playlist = nullptr;

int SoundPlayer::volume() const {
    return m_player->volume();
}

QMediaPlaylist *SoundPlayer::playlist() {
    if (m_playlist == nullptr)
        m_playlist = new QMediaPlaylist();
    return m_playlist;
}

void SoundPlayer::setPlaylist(const Playlist &playlist) {
    m_list.setPlaylist(playlist);
    isPlaylistExist = true;
}

void SoundPlayer::setPlaylist(QSqlQueryModel *model) {
    m_model = model;
    Playlist playlist = handlerPlaylist(m_model);
    setPlaylist(playlist);
}

void SoundPlayer::next() {
    if (!m_list.isMediaSet())
        return;
    m_list.next();
    setPlay();
//    auto test_up = m_list.upNext();
//    auto test_hs = m_list.history();
//    std::cout << " = = = = = = = = =  History  = = = = = = = = = \n";
//    for (int i = test_hs.size() - 1; i >= 0; --i) {
//        std::cout << test_hs[i].getStr().toStdString() << std::endl;
//    }
//    std::cout << " = = = = = = = = =  Up Next  = = = = = = = = = \n";
//    for (int i = 0; i < test_up.size(); i++) {
//        std::cout << test_up[i].getStr().toStdString() << std::endl;
//    }

}

void SoundPlayer::previous() {
    m_list.previous();
    setPlay();
    system("clear");
//    auto test = m_list.upNext();
//    auto test_up = m_list.upNext();
//    auto test_hs = m_list.history();
//    std::cout << " = = = = = = = = =  History  = = = = = = = = = \n";
//    for (int i = test_hs.size() - 1; i >= 0; --i) {
//        std::cout << test_hs[i].getStr().toStdString() << std::endl;
//    }
//    std::cout << " = = = = = = = = =  Up Next  = = = = = = = = = \n";
//    for (int i = 0; i < test_up.size(); i++) {
//        std::cout << test_up[i].getStr().toStdString() << std::endl;
//    }
}


void SoundPlayer::exportPlaylist(const Playlist &playlist, QString path) {
    QMediaPlaylist _pl;

//    auto playlist1 = m_list.currentPlaylist();

    path += ".m3u";

    for (size_t i = 0; i < playlist.size(); ++i) {
        _pl.addMedia(QUrl::fromLocalFile(playlist[i].m_path));
//        std::cout << "URL:: " << playlist1[i].m_path.toStdString() << std::endl;
    }
    if (_pl.save(QUrl::fromLocalFile(path), "m3u")) {
//        std::cout << "To Playlist:: " << path.toStdString() << std::endl;
        ui->statusbar->showMessage("saved", 1200);
    }
    else
        ui->statusbar->showMessage("not saved: " + _pl.errorString(), 1200);
}

void SoundPlayer::importPlaylist(const QString &path) {
    QMediaPlaylist _pl;
    Playlist playlist;
    _pl.load(QUrl::fromLocalFile(path), "m3u");
    playlist.setPlaylistName(QFileInfo(path).fileName());

//    std::cout << "Loaded " << _pl.mediaCount() << "songs :: " <<  _pl.errorString().toStdString() << std::endl;
    for (int i = 0; i < _pl.mediaCount(); ++i) {
        auto media = _pl.media(i).canonicalUrl();
        playlist.addMusic(Music(media));
    }
    emit playlistImported(playlist);

//    return playlist;
//    std::cout << " = = = = = = = = = = Loaded = = = = = = = = = = = " << std::endl;
//    for (size_t i = 0; i < playlist.size(); ++i) {
//        std::cout << playlist[i].getStr().toStdString() << std::endl;
//    }
}

void SoundPlayer::playNext(const Music &song) {
    m_list.addUserNext(song);
}

void SoundPlayer::metaData(bool check) {
//    std::cout << "Avalible: " << check << std::endl;
    if (check) {
        auto title = m_player->metaData(QStringLiteral("Title")).toString();
        ui->labelSong->setText(title.isEmpty() ? "Untitled" : title);
        ui->labelArtist->setText(m_player->metaData(QStringLiteral("ContributingArtist")).toString());
    }
    else {
        auto title = m_list.currentMusic().m_title;
        auto artist = m_list.currentMusic().m_artist;
        ui->labelSong->setText(title);
        ui->labelArtist->setText(artist);
    }
}

void SoundPlayer::modelChanged() {
//    std::cout << "= = = = = = = Reloading = = = = = = = " << std::endl;
    Playlist playlist = handlerPlaylist(m_model);
    m_list.setChangedPlaylist(playlist);
}

Playlist SoundPlayer::handlerPlaylist(QSqlQueryModel *model) {
    Playlist playlist;
//    std::cout << "= = = = = = = Loading = = = = = = = " << std::endl;
    for (int i = 0; i < model->rowCount(); ++i) {
        Music current_song;
        current_song.m_title = model->record(i).value("Title").toString();
        current_song.m_time = model->record(i).value("Time").toString();
        current_song.m_artist = model->record(i).value("Artist").toString();
        current_song.m_rate = model->record(i).value("Rating").toString();
        current_song.m_genre = model->record(i).value("Genre").toString();
        current_song.m_album = model->record(i).value("Album").toString();
        current_song.m_year = model->record(i).value("Year").toString();
        current_song.m_track = model->record(i).value("Track").toString();
        current_song.m_comment = model->record(i).value("Comment").toString();
        current_song.m_name = model->record(i).value("Name").toString();
        current_song.m_path = model->record(i).value("Path").toString();
        current_song.m_cover = model->record(i).value("Cover").toByteArray();
//        std::cout << "  Song : " << current_song.m_title.toStdString() << std::endl;
        playlist.addMusic(current_song);
    }
    return playlist;
}

Music SoundPlayer::getMusicfromTable(QModelIndex index) const {
    Music current_song;
    current_song.m_title = m_model->record(index.row()).value("Title").toString();
    current_song.m_time = m_model->record(index.row()).value("Time").toString();
    current_song.m_artist = m_model->record(index.row()).value("Artist").toString();
    current_song.m_rate = m_model->record(index.row()).value("Rating").toString();
    current_song.m_genre = m_model->record(index.row()).value("Genre").toString();
    current_song.m_album = m_model->record(index.row()).value("Album").toString();
    current_song.m_year = m_model->record(index.row()).value("Year").toString();
    current_song.m_track = m_model->record(index.row()).value("Track").toString();
    current_song.m_comment = m_model->record(index.row()).value("Comment").toString();
    current_song.m_name = m_model->record(index.row()).value("Name").toString();
    current_song.m_path = m_model->record(index.row()).value("Path").toString();
    current_song.m_lyrics = m_model->record(index.row()).value("Lyrics").toString();
    current_song.m_cover = m_model->record(index.row()).value("Cover").toByteArray();
    return current_song;
}

Playlist SoundPlayer::upNext() {
    return m_list.upNext();
}

Playlist SoundPlayer::history() const {
    return m_list.history();
}



