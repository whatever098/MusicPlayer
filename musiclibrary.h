#ifndef __MUSIC_LIBRARY_H__
#define __MUSIC_LIBRARY_H__

// Model - ���ֿ����������

#include "common.h"

class MusicLibrary : public QObject {
    Q_OBJECT

public:
    static MusicLibrary* getInstance(QObject* parent = nullptr);
    // �õ����ڲ��ŵ�������Ϣ
    Song getCurrentSong();

    // ������и���
    QList<Song> getAllSongsFromDatabase();

    bool containsDuplicate(const QList<Song>& songs, const Song& songToCheck);
    // ��ȡ����������Դ
    void fetchOnlineMusic(const QString& folderName);
    // ��ȡ����������Դ
    void fetchLocalMusic(const QString& musicName);

signals:
    void onlineMusicListReady(const QList<Song>& onlineSongs);
    void localMusicListReady(const QList<Song>& localSongs);

private:
    explicit MusicLibrary(QObject* parent = nullptr);

    QSqlDatabase m_database;
    Song currentSong;

    // ����
    void createTables();

    // ��ȡ�����ִ������ݿ�
    void storeMusicListInDatabase(const QList<Song>& songs, const QString& tableName);

    static MusicLibrary* gs_libinstance;
};

#endif // __MUSIC_LIBRARY_H__
