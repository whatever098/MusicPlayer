#include "musicengine.h"
#include "musiclibrary.h"


MusicEngine::MusicEngine(QObject* parent)
    : QObject(parent),
    player(new QMediaPlayer(this)),
    playlist(new QMediaPlaylist(this)),
    m_volume(50),  // ����Ĭ������Ϊ50
    m_isMuted(false), // Ĭ�ϲ�����
    currentPlayMode(PlayMode::Sequential) // Ĭ��˳�򲥷�
{  
    player->setPlaylist(playlist);
    connect(player, &QMediaPlayer::positionChanged, this, &MusicEngine::onPositionChanged);
}

void MusicEngine::onPositionChanged(qint64 position) {
    qint64 duration = player->duration();
    if (duration > 0 && position >= duration) {
        // ��ǰ�������Ž���
        nextSong();
    }
}

// ����
void MusicEngine::play() {
    player->play();
}


void MusicEngine::pause() 
{
    player->pause();
}

void MusicEngine::stop() 
{
    player->stop();
}

void MusicEngine::nextSong() 
{
    switch (currentPlayMode) {
    case PlayMode::Sequential:{ 
        playlist->next();
        player->setPosition(0);
        player->play();
        break;
    }
    case PlayMode::Loop:{
        if (playlist->currentIndex() == (playlist->mediaCount() - 1))
            playlist->setCurrentIndex(0);
        else playlist->next();
        player->setPosition(0);
        player->play();
        break;
    }
    case PlayMode::Random: {
        // ����һ���������
        int randomIndex = qrand() % playlist->mediaCount();
        playlist->setCurrentIndex(randomIndex);
        player->setPosition(0);
        player->play();
        break;
    }
    case PlayMode::RepeatOne: {
        // ����ѭ�����ظ����ŵ�ǰ����
        playlist->setCurrentIndex(playlist->currentIndex());
        player->setPosition(0);
        player->play();
        break;
    }
    default:
        break;
    }
    emit songChanged(songList[playlist->currentIndex()]);
}

void MusicEngine::previousSong() {
    switch (currentPlayMode) {
    case PlayMode::Sequential: {
        playlist->previous();
        player->setPosition(0);
        player->play();
        break;
    }
    case PlayMode::Loop: {
        if (playlist->currentIndex() == 0)
            playlist->setCurrentIndex((playlist->mediaCount() - 1));
        else playlist->previous();
        player->setPosition(0);
        player->play();
        break;
    }
    case PlayMode::Random: {
        // ����һ���������
        int randomIndex = qrand() % playlist->mediaCount();
        playlist->setCurrentIndex(randomIndex);
        player->setPosition(0);
        player->play();
        break;
    }
    case PlayMode::RepeatOne: {
        // ����ѭ�����ظ����ŵ�ǰ����
        playlist->setCurrentIndex(playlist->currentIndex());
        player->setPosition(0);
        player->play();
        break;
    }
    default:
        break;
    }
    emit songChanged(songList[playlist->currentIndex()]);
}

void MusicEngine::playSong(const Song& song) {
    songList.append(song);
    QString songPath;
    if(song.getPath()[0] != '.')    
        songPath = "./localcache" + song.getPath() + song.getMp3();
    else 
        songPath = song.getPath() + song.getMp3();
    QMediaContent mediaContent(QUrl::fromLocalFile(songPath));

    // ��ָ��������ӵ������б���
    playlist->addMedia(mediaContent);

    // ���ò�������ǰý��Ϊ�����б��������ӵĸ���
    player->setPlaylist(playlist);
    playlist->setCurrentIndex(playlist->mediaCount() - 1);
    player->play();
}

int MusicEngine::volume() const 
{
    return m_volume;
}

void MusicEngine::setVolume(int volume) 
{
    m_volume = volume;
    player->setVolume(volume);
}

bool MusicEngine::isMuted() const 
{
    return m_isMuted;
}

void MusicEngine::setMuted(bool muted) 
{
    m_isMuted = muted;
    player->setMuted(muted);
}

bool MusicEngine::isPlaying() const 
{
    return (player->state() == QMediaPlayer::PlayingState);
}

bool MusicEngine::isPaused() const 
{
    return (player->state() == QMediaPlayer::PausedState);
}

qint64 MusicEngine::duration() const 
{
    return player->duration();
}

qint64 MusicEngine::position() const 
{
    return player->position();
}

PlayMode MusicEngine::getCurrentPlayMode() {
    return currentPlayMode;
}

void MusicEngine::setPlayMode()
{
    switch (currentPlayMode) {
    case PlayMode::Sequential:
        currentPlayMode = PlayMode::Loop;
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case PlayMode::Loop:
        currentPlayMode = PlayMode::Random;
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        break;
    case PlayMode::Random:
        currentPlayMode = PlayMode::RepeatOne;
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        break;
    case PlayMode::RepeatOne:
        currentPlayMode = PlayMode::Sequential;
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    default:
        break;
    }
}

// ���ò������Ĳ���λ��
void MusicEngine::setPosition(qint64 position) {
    if (player) {
        player->setPosition(position);
    }
}