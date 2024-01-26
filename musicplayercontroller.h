#ifndef __MUSIC_PLAYER_CONTROLLER_H__
#define __MUSIC_PLAYER_CONTROLLER_H__

// Controller - ���ֲ�����������,����ģ�ͺ���ͼ
// ����ģʽʵ��

#include "musicengine.h"
#include "musiclibrary.h"
#include "localcache.h"
#include "musicplayerui.h"
#include "lyricsdisplaywidget.h"
#include "musiclistwidget.h"

class MusicPlayerController : public QObject {
    Q_OBJECT

public:
    MusicPlayerController(MusicEngine* eng, LocalCache* cach, MusicPlayerUI* v, MusicListWidget* ml);
    void showImage(const Song& selectedSong);

private slots:
    // ����������ͼ��ģ�͵��ź�
    void handlePlayButtonClicked();
    void handleNextButtonClicked();
    void handlePreviousButtonClicked();
    void handleSliderMoved(qint64 position);
    void handleVolumeChanged(qint64 volume);
    void handleModeChanged();
    void handleLocalMusicList(QList<Song> localMusicList);
    void handleOnlineMusicList(QList<Song> onlineMusicList);
    void handleLocalPlaySong(const Song& selectedSong);
    void handleOnlinePlaySong(const Song& selectedSong);
    void handleUpdateLyrics();
    void handleUpdatePrograss();
    void handleSongChanged(Song& song);
    void handleSearchOnline(const QString& folderName);
    void handleAddLocalMusic(const Song& localSong);

private:
    // ˽�г�Ա����
    QTimer* timer;
    MusicEngine* engine;
    LocalCache* cache;
    MusicPlayerUI* view;
    MusicListWidget* musicList;
    MusicLibrary* lib = MusicLibrary::getInstance();
};

#endif // __MUSIC_PLAYER_CONTROLLER_H__
