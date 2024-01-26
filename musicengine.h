#ifndef __MUSIC_ENGINE_H__
#define __MUSIC_ENGINE_H__

// Model - ��������

#include "common.h"

class MusicEngine : public QObject {
    Q_OBJECT

public:
    MusicEngine(QObject* parent = nullptr);
    // ���ſ���
    void play();
    void pause();
    void stop();
    void nextSong();
    void previousSong();
    void playSong(const Song& index);
    void setPlayMode();
    void setPosition(qint64 position);

    // ��������
    int volume() const;
    void setVolume(int volume);
    bool isMuted() const;
    void setMuted(bool muted);

    // ����״̬
    bool isPlaying() const;
    bool isPaused() const;

    // ������Ϣ
    qint64 duration() const;
    qint64 position() const;

    PlayMode getCurrentPlayMode();

signals:
    void songChanged(Song& song);

private slots:
    void onPositionChanged(qint64 position);

private:
    QMediaPlayer* player;
    QMediaPlaylist* playlist; // �����б�
    QList<Song> songList; // ��Ӧ�����б�ĸ�����Ϣ�б�
    PlayMode currentPlayMode; // ��ǰ����ģʽ
    int m_volume; // ����
    bool m_isMuted; // �Ƿ���
};

#endif // __MUSIC_ENGINE_H__
