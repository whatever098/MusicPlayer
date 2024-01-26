#ifndef __MUSIC_LIBRARY_H__
#define __MUSIC_LIBRARY_H__

// Model - 音乐库管理（单例）

#include "common.h"

class MusicLibrary : public QObject {
    Q_OBJECT

public:
    static MusicLibrary* getInstance(QObject* parent = nullptr);
    // 得到正在播放的音乐信息
    Song getCurrentSong();

    // 获得所有歌曲
    QList<Song> getAllSongsFromDatabase();

    bool containsDuplicate(const QList<Song>& songs, const Song& songToCheck);
    // 获取线上音乐资源
    void fetchOnlineMusic(const QString& folderName);
    // 获取本地音乐资源
    void fetchLocalMusic(const QString& musicName);

signals:
    void onlineMusicListReady(const QList<Song>& onlineSongs);
    void localMusicListReady(const QList<Song>& localSongs);

private:
    explicit MusicLibrary(QObject* parent = nullptr);

    QSqlDatabase m_database;
    Song currentSong;

    // 建表
    void createTables();

    // 获取的音乐存入数据库
    void storeMusicListInDatabase(const QList<Song>& songs, const QString& tableName);

    static MusicLibrary* gs_libinstance;
};

#endif // __MUSIC_LIBRARY_H__
