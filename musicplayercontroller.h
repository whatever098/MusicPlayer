#ifndef __MUSIC_PLAYER_CONTROLLER_H__
#define __MUSIC_PLAYER_CONTROLLER_H__

// Controller - 音乐播放器控制器,连接模型和视图
// 单例模式实现

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
    // 处理来自视图或模型的信号
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
    // 私有成员变量
    QTimer* timer;
    MusicEngine* engine;
    LocalCache* cache;
    MusicPlayerUI* view;
    MusicListWidget* musicList;
    MusicLibrary* lib = MusicLibrary::getInstance();
};

#endif // __MUSIC_PLAYER_CONTROLLER_H__
