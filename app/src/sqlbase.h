#ifndef UAMP_SQLBASE_H
#define UAMP_SQLBASE_H

#include <QString>
#include <QSqlDatabase>
#include "music.h"

class SqlBase {

public:
   SqlBase();
   ~SqlBase();

   bool AddtoLibrary(const QString& media_path);
   bool AddNewPlaylist(const QString& name);
   bool DeletePlaylist(const QString& name);
   bool AddtoPlaylist(const QString& path, const QString& cur_playlist);

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