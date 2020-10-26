#include "settings.h"
#include "app.h"
#include "mainwindow.h"
//#include "sound_tags.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <algorithm>
#include <QtWidgets>
#include "loggingcategories.h"
#include "dialogpreferences.h"
#include "dialoginfo.h"

//    {"Name", "Time", "Title", "Artist", "Genre", "Album", "Year", "Track", "Path", "Comment" };

#define default_cover ":/logo1.png"

using std::cout;
using std::endl;

char *toChar2(QString str)
{
    char *test = str.toUtf8().data();
    return test;
}

//void MainWindow::outputCurrentInfo(const QVector<QString> &current, const QModelIndex &index)
//{
//    m_tableViewer->setNewItems(current, index);
//}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();
    m_settings = new Settings();

    m_player = new SoundPlayer(ui);

    m_library = new MediaLibrary();

    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    QImage def_cover(":/def_cover_color.png");
    QPixmap pix(QPixmap::fromImage(def_cover));
//    ui->cover_label_large->setPixmap(pix);
    ui->cover_label->setPixmap(pix);
    ui->sideBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->sideBar, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onSideBarContextMenu(const QPoint &)));

//    m_searcher = new Searcher{ui->search_line, ui->filterBox, &m_music_list};
    ui->statusbar->hide();
    connect(ui->filterBox, SIGNAL(activated(int)), this, SLOT(on_search_line_editingFinished()));
}

void MainWindow::onSideBarContextMenu(const QPoint &point)
{
    QMenu contextMenu(tr("SideBar context menu"), this);

//    auto fullFileName = dynamic_cast<QFileSystemModel *>(ui->treeView->model())->filePath(ui->treeView->indexAt(point));

    QAction action_new("New Playlist", this);
    connect(&action_new, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_new);

//    QAction action_rename("Rename ", this);
//    connect(&action_rename, &QAction::triggered, this, [=] () { on_action_context_file_rename(fullFileName); });
//    contextMenu.addAction(&action_rename);
//
//    QAction action_delete("Delete ", this);
//    connect(&action_delete, &QAction::triggered, this, [=] { on_action_context_file_delete(fullFileName) ;});
//    contextMenu.addAction(&action_delete);

    contextMenu.exec(mapToGlobal(point));
}


void MainWindow::on_actionPlaylist_triggered()
{
    bool ok;
    QString new_playlist_name = QInputDialog::getText(this,"New folder",
                                                    tr("Enter the new path for the new folder"),
                                                    QLineEdit::Normal,
                                                    "",
                                                    &ok);
    if (ok) {
            qInfo(logInfo()) << "new playlist " << new_playlist_name;
        } else {
//            QMessageBox::warning(0, "Warning ", "You cannot delete the folder");  // show warning
        qInfo(logInfo()) << "Warning You cannot delete the folder " << new_playlist_name;
    }
}



MainWindow::~MainWindow()
{
//    delete m_searcher;
    delete  m_library;
    delete ui;
    system("leaks -q uamp");
}

void MainWindow::on_mainMusicTable_clicked(const QModelIndex &index)
{
    m_table_index = index;

}



//void MainWindow::on_pushButton_clicked()
//{
//    auto newSongTag = m_tableViewer->getResult();
//
//    if (newSongTag[8].isEmpty() || newSongTag[0].isEmpty()) {
//        return;
//    }
//    if (!(modify_tags(newSongTag))) {
//        ui->statusbar->showMessage(newSongTag[8] + " is not writable", 200);
////        m_log->add_log_massage(newSongTag[8] + " is not writable");
//    }
//
//    m_music_list[m_tableViewer->getIndex().row()] = std::move(newSongTag);
//    m_tableModel->music_list_add(m_music_list);
//    ui->mainMusicTable->setModel(m_tableModel);
//    ui->mainMusicTable->viewport()->update();
//}

void MainWindow::on_mainMusicTable_doubleClicked(const QModelIndex &index)
// player
{
    QVector<QString> current = m_library->data()[index.row()];

    //    QImage coverQImg = load_cover_image(m_music_list[index.row()][8].toStdString().data());
    //    ui->statusbar->showMessage(tr("image loaded"), 200);
    //    QGraphicsScene *scene = new QGraphicsScene();
    //    ui->imageSong->setScene(scene);
    //    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(coverQImg));
    //    scene->addItem(item);
    //    ui->imageSong->show();
//    outputCurrentInfo(current, index);
    setMusicPlay(current[8]);

    std::string fileName = m_library->data()[index.row()][0].toStdString().data();
    std::string fileType = fileName.substr(fileName.size() - 3);

    QImage coverQImg;

    if (fileType == "mp3") {
        coverQImg = load_cover_image_mpeg(m_library->data()[index.row()][8].toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else if (fileType == "m4a") {
        coverQImg = load_cover_image_m4a(m_library->data()[index.row()][8].toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else {
        ui->statusbar->showMessage(tr(" cover is unsupported." ), 200);
//        coverQImg = QImage("../../app/logo1.png");  // Clion
        coverQImg = QImage("./app/logo1.png");  // zsh
    }

    QPixmap pix(QPixmap::fromImage(coverQImg));
    ui->cover_label->setPixmap(pix);
}


void MainWindow::setMusicPlay(QString soundPath)
{
    m_player->setSound(soundPath);
}

void MainWindow::on_playButton_clicked()
{
    m_player->setPlay();
}

void MainWindow::on_pauseButton_clicked()
{
    m_player->setPause();
}

void MainWindow::on_stopButton_clicked()
{
    m_player->setStop();
}

void MainWindow::on_statusPlay_valueChanged(int value)
{
    m_player->setMovedPosition(value);
}

void MainWindow::on_statusVolume_valueChanged(int value)
{
    m_player->setVolume(value);
}


/*
void MainWindow::on_change_cover_button_clicked()
{
    auto currentSongTag = m_tableViewer->getResult();
    if (currentSongTag[8].isEmpty() || currentSongTag[0].isEmpty()) {
        return;
    }

    std::string current_file = currentSongTag[0].toStdString();
    std::string fileType = current_file.substr(current_file.size() - 3);

    if (fileType == "mp3") {
        QString file_image = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "~/",
                tr("Images (*.png *.jpg)")
        );
        if (!(set_image_mpeg(currentSongTag[8].toStdString().data(), file_image.toStdString().data()))) {
//            m_log->add_log_massage(currentSongTag[8] + " not editable");
            ui->statusbar->showMessage(currentSongTag[0] + "not editable", 2000);
        }
        ui->statusbar->showMessage(tr(file_image.toStdString().data()), 2000);
    }

//    else if (fileType == "m4a") {
//        QString file_image = QFileDialog::getOpenFileName(
//                this,
//                tr("Open File"),
//                "~/",
//                tr("Images (*.png *.jpg)")
//        );
//        if (!(load_cover_image_m4a(currentSongTag[8].toStdString().data(), file_image.toStdString().data()))) {
//            m_log->add_log_massage(currentSongTag[8] + " not editable");
//            ui->statusbar->showMessage(currentSongTag[0] + "not editable", 2000);
//        }
//        ui->statusbar->showMessage(tr(file_image.toStdString().data()), 2000);
//    }

    else {
        ui->statusbar->showMessage(tr("cover not editable"), 2000);
    }
}


*/
//void MainWindow::on_search_line_editingFinished()
//{
////    readDir(ui->fileBrowser->currentIndex());
//    auto tmp = m_searcher->search();
//    m_music_list = tmp;
//    if (!m_tableModel)
//        delete m_tableModel;
//    m_tableModel = new MusicTableModel(ui->mainMusicTable);
//    m_tableModel->music_list_add(m_music_list);
//    ui->mainMusicTable->setModel(m_tableModel);
//    ui->mainMusicTable->viewport()->update();
//}

void MainWindow::readSettings() {
    QSettings *settings = App::get_app()->app_settings();

//    QString theme = settings->value("theme").toString();
    const QByteArray geometry = settings->value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings() {
    qInfo(logInfo()) << "write settings before quit";

    QSettings *settings = App::get_app()->app_settings();
    settings->setValue("geometry", saveGeometry());
}


void MainWindow::on_actionQuit_triggered()
{
    writeSettings();
    qInfo(logInfo()) << QString("Quit uamp\n");
    App::quit();
}

void MainWindow::on_actionPreferences_triggered()
{
    qDebug(logDebug()) << "on_action Preferences_triggered";

    DialogPreferences *window_settings = new DialogPreferences(m_settings->get_current_settings(), 0);
    window_settings->setModal(true);

//    QObject::connect(window_settings, &DialogSettings::SavedSettings, this, &MainWindow::applySettings);
    if (window_settings->exec() == QDialog::Accepted) {
        qInfo(logInfo()) << "exec QDilaog";
        QMap<QString, QString> new_settings;
        window_settings->get_dialog_options(new_settings);  // get settings from QDialog
//        m_settings->set_settings(new_settings);
    }
}

void MainWindow::on_actionInfo_triggered()
{
  qDebug(logDebug()) << "on_actionInfo_triggered";

  QVector<QString> current = m_library->data()[m_table_index.row()];

  DialogInfo *songInfo = new DialogInfo(current, 0);
//  songInfo->setWindowFlags(Qt::CustomizeWindowHint);
  songInfo->setModal(true);

  if (songInfo->exec() == QDialog::Accepted) {
    qInfo(logInfo()) << "ok DialogInfo";

  }
  else {
    qInfo(logInfo()) << "cancel DialogInfo";
  }
}

void MainWindow::on_actionAdd_to_Library_triggered()  // add folders
{
        QString f_name = QFileDialog::getExistingDirectory(this, "Add media", "");
//                                                        QFileDialog::ShowDirsOnly
//                                                        | QFileDialog::DontResolveSymlinks);
    m_library->add_media(f_name);

    if (!m_tableModel)
        delete m_tableModel;
    m_tableModel = new MusicTableModel(ui->mainMusicTable);
    m_tableModel->music_list_add(m_library->data());
    ui->mainMusicTable->setModel(m_tableModel);
    ui->mainMusicTable->viewport()->update();
}















/*
QFileDialog* _f_dlg = new QFileDialog(this);
_f_dlg->setFileMode(QFileDialog::Directory);
_f_dlg->setOption(QFileDialog::DontUseNativeDialog, true);

// Try to select multiple files and directories at the same time in QFileDialog
QListView *l = _f_dlg->findChild<QListView*>("listView");
if (l) {
    l->setSelectionMode(QAbstractItemView::MultiSelection);
}
QTreeView *t = _f_dlg->findChild<QTreeView*>();
if (t) {
    t->setSelectionMode(QAbstractItemView::MultiSelection);
}

//    int nMode =
    if (_f_dlg->exec()) {
        QStringList _fnames = _f_dlg->selectedFiles();
        qDebug(logDebug()) << "add media " << _fnames.size();
        for (int i = 0; i < _fnames.size(); ++i) {
            m_library->add_media(_fnames.at(i));
        }


        if (!m_tableModel)
            delete m_tableModel;
        m_tableModel = new MusicTableModel(ui->mainMusicTable);
//            m_searcher->setDown();
        m_tableModel->music_list_add(m_library->data());
        ui->mainMusicTable->setModel(m_tableModel);
    }
    */