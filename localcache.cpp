#include "localcache.h"


// �������������
void LocalCache::cacheMusic(const Song& song) {
    QString baseUrl = "https://gitee.com/MarkYangUp/music/raw/master";

    QString songPath = song.getPath();

    // �����ļ������� URL
    QString mp3Url = QUrl(baseUrl + songPath + song.getMp3()).toEncoded(QUrl::FullyEncoded);
    QString lyricUrl = QUrl(baseUrl + songPath + song.getLyric()).toEncoded(QUrl::FullyEncoded);
    QString imgUrl = QUrl(baseUrl + songPath + song.getImg()).toEncoded(QUrl::FullyEncoded);

    // ��������Ŀ¼
    QString localPath = "./localcache/" + songPath;
    QDir().mkpath(localPath);

    // ��ȡ�ϼ�Ŀ¼��·��
    QString parentDir = QFileInfo(baseUrl + songPath + "music.json").dir().path();

    // �Ƴ����һ��·�����֣��õ���һ��Ŀ¼��·��
    QString grandParentDir = QFileInfo(parentDir).dir().path();

    // ���� music.json �� URL
    QString jsonUrl = QDir(grandParentDir).filePath("music.json");
    jsonUrl = QUrl(jsonUrl).toEncoded(QUrl::FullyEncoded);
    QString jsonPath = QDir(grandParentDir).filePath("music.json");
    jsonPath.remove(0, 45);
    jsonPath = "./localcache" + jsonPath;
    downloadFile(jsonUrl, jsonPath);

    // ���ز������ļ�
    downloadFile(mp3Url, localPath + "/" + song.getMp3());
    downloadFile(lyricUrl, localPath + "/" + song.getLyric());
    downloadFile(imgUrl, localPath + "/" + song.getImg());

    emit addLocalMusic(song);
}


void LocalCache::downloadFile(const QString& fileUrl, const QString& outputPath) {
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(fileUrl)));

    // �ȴ�������ɻ�ʱ
    while (!reply->isFinished()) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3000); // �����¼�ѭ����3000msΪ��ʱʱ��
    }

    if (reply->error() == QNetworkReply::NoError) {
        // �ɹ��������ļ�
        QFile file(outputPath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
        }
    }
    else {
        // �������
        qDebug() << "�ļ����س���" << reply->errorString();
    }
    reply->deleteLater();
}


// ɾ�������еĸ����ļ�
void LocalCache::deleteCachedMusic(const Song& song) {
    QString cachePath = cacheFolderPath + "/" + song.getMusicName(); // ���������ڻ����е�·��

    QFile cachedMusic(cachePath);
    if (cachedMusic.exists()) {
        cachedMusic.remove();
    }
}
