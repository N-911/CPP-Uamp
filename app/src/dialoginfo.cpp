#include <QFileDialog>
#include "dialoginfo.h"
#include "ui_dialoginfo.h"
#include "tag_functions.h"
#include "loggingcategories.h"
#include "string"

DialogInfo::DialogInfo(Music songInfo, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);
    m_tagsInfo = songInfo;

    qDebug(logDebug()) << "DialogInfo constuctor";

    ui->line_title->setText(songInfo.m_title);
    ui->line_artist->setText(songInfo.m_artist);
    ui->line_album->setText(songInfo.m_album);
    ui->line_genre->setText(songInfo.m_genre);
    ui->line_year->setText(QString::number(songInfo.m_year));
    ui->line_track->setText(QString::number(songInfo.m_track));
    ui->line_comments->setText(songInfo.m_comment);
    ui->line_path->setText(songInfo.m_path);
    ui->title_large->setText(songInfo.m_title);
    ui->artisti_large->setText(songInfo.m_artist);

    connect(ui->coverInfo, &ClickedLabel::doubleClicked, this, &DialogInfo::coverInfoDoubleclicked);
    load_cover();
}


void DialogInfo::load_cover() {
    std::string fileName = m_tagsInfo.m_name.toStdString();
    std::string fileType = fileName.substr(fileName.size() - 3);

    QImage coverQImg;
    if (fileType == "mp3") {
        coverQImg = load_cover_image_mpeg(m_tagsInfo.m_path.toStdString().data());
//        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else if (fileType == "m4a") {
        coverQImg = load_cover_image_m4a(m_tagsInfo.m_path.toStdString().data());
//        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else {
//        ui->statusbar->showMessage(tr(" cover is unsupported"), 200);
        coverQImg = QImage(":/def_cover_color.png");
//        coverQImg = QImage(default_cover);
    }
    QPixmap pix(QPixmap::fromImage(coverQImg));
    ui->coverInfo->setPixmap(pix);
}


DialogInfo::~DialogInfo()
{
    delete ui;
}



void DialogInfo::coverInfoDoubleclicked() {
    qDebug(logDebug()) << "coverInfoDoubleclicked";

    if (m_tagsInfo.m_path.isEmpty() || m_tagsInfo.m_name.isEmpty()) {
        return;
    }

    std::string current_file = m_tagsInfo.m_name.toStdString();
    std::string fileType = current_file.substr(current_file.size() - 3);

    if (fileType == "mp3") {
        QString file_image = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "~/",
                tr("Images (*.png *.jpg)")
        );
        qDebug(logDebug()) << "new file name cover" << file_image;

        if (!(set_image_mpeg(m_tagsInfo.m_path.toStdString().data(), file_image.toStdString().data()))) {
            qInfo(logInfo()) << m_tagsInfo.m_name <<  " not editable";
//            ui->statusbar->showMessage(m_tagsInfo[0] + "not editable", 2000);
        }
        load_cover();  // reopen cover
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
        qInfo(logInfo()) << "cover not editable";
    }
}

void DialogInfo::accepted() {
//    qDebug(logDebug()) << "ui->themeComboBox->currentText()" << ui->themeComboBox->currentText();
//    m_dialog_settings["theme"] = ui->themeComboBox->currentText();
//
//    emit SavedSettings(m_dialog_settings);
    QDialog::accept();
}

Music DialogInfo::get_tag_changes(Music &music_tags) {
    music_tags.m_name = ui->line_title->text();
    music_tags.m_artist = ui->line_artist->text();
    music_tags.m_album = ui->line_album->text();
    music_tags.m_genre = ui->line_genre->text();

    music_tags.m_year = std::stoi(ui->line_year->text().toStdString());
    music_tags.m_track = std::stoi(ui->line_track->text().toStdString());

    music_tags.m_comment = ui->line_comments->text();
    music_tags.m_path = m_tagsInfo.m_path;
    return Music();
}




//void DialogInfo::on_nextInfo_clicked()
//{
//
//}



