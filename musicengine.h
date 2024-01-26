#ifndef __MUSIC_ENGINE_H__
#define __MUSIC_ENGINE_H__

// Model - 音乐引擎

#include "common.h"

class MusicEngine : public QObject {
    Q_OBJECT

public:
    MusicEngine(QObject* parent = nullptr);
    // 播放控制
    void play();
    void pause();
    void stop();
    void nextSong();
    void previousSong();
    void playSong(const Song& index);
    void setPlayMode();
    void setPosition(qint64 position);

    // 音量控制
    int volume() const;
    void setVolume(int volume);
    bool isMuted() const;
    void setMuted(bool muted);

    // 播放状态
    bool isPlaying() const;
    bool isPaused() const;

    // 歌曲信息
    qint64 duration() const;
    qint64 position() const;

    PlayMode getCurrentPlayMode();

signals:
    void songChanged(Song& song);

private slots:
    void onPositionChanged(qint64 position);

private:
    QMediaPlayer* player;
    QMediaPlaylist* playlist; // 播放列表
    QList<Song> songList; // 对应播放列表的歌曲信息列表
    PlayMode currentPlayMode; // 当前播放模式
    int m_volume; // 音量
    bool m_isMuted; // 是否静音
};

#endif // __MUSIC_ENGINE_H__
