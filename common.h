#ifndef __COMMON_H__
#define __COMMON_H__

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QTextCodec>
#include <QtCore/QMetaType>
#include <QtCore/QTimer>
#include <QtCore/QCoreApplication>
#include <QtCore/QEventLoop>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMenu>
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include <QtGui/QCloseEvent>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <algorithm>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MusicPlayerUI;
    class MainWindow;
    class MusicList;
}
QT_END_NAMESPACE

static QNetworkAccessManager* gs_networkManager = new QNetworkAccessManager();// 网络管理单例

// 定义播放模式的枚举
enum class PlayMode {
    Sequential, // 顺序播放
    Loop, // 列表循环
    Random, // 随机播放 
    RepeatOne // 单曲循环
};

// 定义音乐格式的枚举
enum class MusicFormat {
    MP3,
    WAV,
    FLAC,
    AAC,
    OGG
};

// 歌曲信息结构
class Song {
private:
    QString musicName;
    QString albumName;
    int duration;
    // 文件夹路径
    QString path;

    // 各文件路径
    QString mp3;
    QString lyric;
    QString img;

public:
    // 构造函数
    Song(const QString& name, const QString& album, const int& dur, const QString& pth,
        const QString& audio, const QString& lyrics, const QString& image)
        : musicName(name), albumName(album), duration(dur), path(pth), mp3(audio), lyric(lyrics), img(image) {}

    // 默认构造函数
    Song() {};

    QString getMusicName() const {
        return musicName;
    }
    QString getAlbumName() const {
        return albumName;
    }
    QString getDuration() const {
        return duration;
    }
    QString getPath() const {
        return path;
    }
    QString getMp3() const {
        return mp3;
    }
    QString getLyric() const {
        return lyric;
    }
    QString getImg() const {
        return img;
    }
    // 重载==运算符便于QList操作
    bool operator==(const Song& other) const {
        return (musicName == other.musicName && albumName == other.albumName);
    }
};

// 歌曲列表展示信息
struct SongItemData {
    Song song;
    QString displayText;
};

Q_DECLARE_METATYPE(Song)
Q_DECLARE_METATYPE(SongItemData)

#endif // __COMMON_H__
