#ifndef UAMP_SQLBASE_H
#define UAMP_SQLBASE_H

#include <QString>
#include <QSqlDatabase>
#include "music.h"
#include "playlist.h"

class SqlBase {

public:
   SqlBase();
   ~SqlBase();

   bool AddtoLibrary(const QString& media_path);
   bool AddNewPlaylist(const QString& name);
   bool DeletePlaylist(const QString& name);
   Playlist ExportPlaylist(const QString& name);
   bool AddtoPlaylist(const QString& path, const QString& cur_playlist);

public slots:
    bool insertIntoTable(const QVariantList &data);      // Добавление записей в таблицу
    bool insertIntoTable(const QString &name, const QByteArray &pic);

private:
    bool createConnection();
    bool createNewBase();
    bool createTableSongs();
    bool createTableListPlaylists();
    bool createTablePlaylist();
    bool loadData();
    QSqlDatabase m_media_base;
};


#endif //UAMP_SQLBASE_H
