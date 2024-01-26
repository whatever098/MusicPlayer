#include "localcache.h"


// 缓存歌曲到本地
void LocalCache::cacheMusic(const Song& song) {
    QString baseUrl = "https://gitee.com/MarkYangUp/music/raw/master";

    QString songPath = song.getPath();

    // 构建文件的完整 URL
    QString mp3Url = QUrl(baseUrl + songPath + song.getMp3()).toEncoded(QUrl::FullyEncoded);
    QString lyricUrl = QUrl(baseUrl + songPath + song.getLyric()).toEncoded(QUrl::FullyEncoded);
    QString imgUrl = QUrl(baseUrl + songPath + song.getImg()).toEncoded(QUrl::FullyEncoded);

    // 创建本地目录
    QString localPath = "./localcache/" + songPath;
    QDir().mkpath(localPath);

    // 获取上级目录的路径
    QString parentDir = QFileInfo(baseUrl + songPath + "music.json").dir().path();

    // 移除最后一个路径部分，得到上一级目录的路径
    QString grandParentDir = QFileInfo(parentDir).dir().path();

    // 构建 music.json 的 URL
    QString jsonUrl = QDir(grandParentDir).filePath("music.json");
    jsonUrl = QUrl(jsonUrl).toEncoded(QUrl::FullyEncoded);
    QString jsonPath = QDir(grandParentDir).filePath("music.json");
    jsonPath.remove(0, 45);
    jsonPath = "./localcache" + jsonPath;
    downloadFile(jsonUrl, jsonPath);

    // 下载并保存文件
    downloadFile(mp3Url, localPath + "/" + song.getMp3());
    downloadFile(lyricUrl, localPath + "/" + song.getLyric());
    downloadFile(imgUrl, localPath + "/" + song.getImg());

    emit addLocalMusic(song);
}


void LocalCache::downloadFile(const QString& fileUrl, const QString& outputPath) {
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(fileUrl)));

    // 等待请求完成或超时
    while (!reply->isFinished()) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3000); // 处理事件循环，3000ms为超时时间
    }

    if (reply->error() == QNetworkReply::NoError) {
        // 成功，保存文件
        QFile file(outputPath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
        }
    }
    else {
        // 处理错误
        qDebug() << "文件下载出错：" << reply->errorString();
    }
    reply->deleteLater();
}


// 删除缓存中的歌曲文件
void LocalCache::deleteCachedMusic(const Song& song) {
    QString cachePath = cacheFolderPath + "/" + song.getMusicName(); // 构建歌曲在缓存中的路径

    QFile cachedMusic(cachePath);
    if (cachedMusic.exists()) {
        cachedMusic.remove();
    }
}
