#include "musiclibrary.h"


MusicLibrary* MusicLibrary::gs_libinstance = nullptr;

MusicLibrary::MusicLibrary(QObject* parent) : QObject(parent){
    QString databasePath = "./database.db";

    // 添加 SQLite 数据库
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(databasePath);

    if (!m_database.open()) {
        qDebug() << "错误：无法创建/打开数据库：" << m_database.lastError().text();
    }
    else {
        qDebug() << "成功：数据库已创建/打开！";
        createTables();
    }
}

MusicLibrary* MusicLibrary::getInstance(QObject* parent) {
    if (!gs_libinstance) {
        gs_libinstance = new MusicLibrary(parent);
    }
    return gs_libinstance;
}

void MusicLibrary::createTables() {
    QSqlQuery query(m_database);

    QString createTableQuery = "CREATE TABLE IF NOT EXISTS Songs ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "MusicName TEXT NOT NULL,"
        "AlbumName TEXT NOT NULL,"
        "Duration TEXT,"
        "Path TEXT NOT NULL,"
        "MP3 TEXT NOT NULL,"
        "Lyric TEXT,"
        "Image TEXT"
        ")";

    if (!query.exec(createTableQuery)) {
        qDebug() << "错误创建表格：" << query.lastError().text();
    }
    else {
        qDebug() << "表格成功创建！";
    }
}

void MusicLibrary::fetchOnlineMusic(const QString& folderName) {
    QString encodedFolderName = QUrl::toPercentEncoding(folderName.toUtf8());
    QString baseUrl = "https://gitee.com/MarkYangUp/music/raw/master/" + encodedFolderName + "/";

    QNetworkReply* reply = gs_networkManager->get(QNetworkRequest(QUrl(baseUrl + "music.json")));

    QObject::connect(reply, &QNetworkReply::finished, [this, reply, baseUrl]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray jsonData = reply->readAll();

            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (!doc.isNull()) {
                QJsonObject json = doc.object();
                QJsonArray musicList = json["list"].toArray();

                QList<Song> onlineSongs;

                for (const QJsonValue& value : musicList) {
                    QJsonObject songObj = value.toObject();

                    QString musicName = songObj["musicName"].toString();
                    QString albumName = songObj["albumName"].toString();
                    int duration = songObj["duration"].toInt(); 
                    QString path = songObj["path"].toString() + "/";
                    QString mp3 = songObj["mp3"].toString();
                    QString lyric = songObj["lyric"].toString();
                    QString img = songObj["img"].toString();

                    Song song(musicName, albumName, duration, path, mp3, lyric, img); 
                    onlineSongs.append(song);
                }

                storeMusicListInDatabase(onlineSongs, "online_music");
                emit onlineMusicListReady(onlineSongs);
            }
        }
        else {
            qDebug() << "网络请求失败：" << reply->errorString();
        }

        reply->deleteLater();
        });
}

void MusicLibrary::fetchLocalMusic(const QString& folderName) {
    QList<QString> foldersToCheck;
    foldersToCheck.append(folderName);

    QList<Song> songs;

    QStringList filters;
    filters << "*.mp3" << "*.wav" << "*.flac" << "*.aac"; // 添加所需的文件格式

    while (!foldersToCheck.isEmpty()) {
        QString currentFolder = foldersToCheck.takeFirst();
        QDir directory(currentFolder);

        if (!directory.exists()) {
            qDebug() << "文件夹不存在。";
            continue;
        }

        QStringList musicFiles = directory.entryList(filters, QDir::Files | QDir::NoSymLinks);

        for (const QString& file : musicFiles) {
            QString filePath = directory.filePath(file);
            QDir parentDir = directory;
            parentDir.cdUp(); // 切换到 mp3 文件所在目录的上级目录

            QString parentDirPath = parentDir.absolutePath();
            QString musicJsonPath = parentDirPath + QDir::separator() + "music.json";

            if (QFile::exists(musicJsonPath)) {
                // music.json 存在，从中读取音乐信息
                QFile musicJsonFile(musicJsonPath);
                if (musicJsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QJsonParseError error;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(musicJsonFile.readAll(), &error);

                    if (error.error == QJsonParseError::NoError) {
                        QJsonObject jsonObject = jsonDoc.object();
                        QJsonArray musicList = jsonObject["list"].toArray();

                        for (const auto& musicItem : musicList) {
                            QJsonObject songObj = musicItem.toObject();

                            Song song(songObj["musicName"].toString(), songObj["albumName"].toString(),
                                songObj["duration"].toInt(), "./localcache" + songObj["path"].toString() + "/",
                                songObj["mp3"].toString(), songObj["lyric"].toString(),
                                songObj["img"].toString());
                            if (!containsDuplicate(songs, song)) {
                                songs.append(song);
                            }
                        }
                    }
                    musicJsonFile.close();
                }
            }
            else {
                // music.json 不存在，按原方式构造 Song 对象
                Song song(file, "", 0, filePath, "", "", "");
                songs.append(song);
            }
        }
        QStringList dirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& dir : dirs) {
            QString subdir = currentFolder + QDir::separator() + dir;
            foldersToCheck.append(subdir);
        }
    }

    storeMusicListInDatabase(songs, "local_music");
    emit localMusicListReady(songs);
}

bool MusicLibrary::containsDuplicate(const QList<Song>& songs, const Song& songToCheck) {
    for (const Song& song : songs) {
        if (song.getMusicName() == songToCheck.getMusicName() &&
            song.getAlbumName() == songToCheck.getAlbumName()) {
            return true; // 如果找到相同的歌曲则返回 true
        }
    }
    return false; // 如果列表中没有相同的歌曲则返回 false
}

void MusicLibrary::storeMusicListInDatabase(const QList<Song>& songs, const QString& tableName) {
    QSqlQuery query;

    query.prepare("INSERT INTO " + tableName + "(MusicName, AlbumName, Duration, Path, MP3, Lyric, Image) "
        "VALUES (:musicName, :albumName, :duration, :path, :mp3, :lyric, :img)");

    for (const Song& song : songs) {
        query.bindValue(":musicName", song.getMusicName());
        query.bindValue(":albumName", song.getAlbumName());
        query.bindValue(":duration", song.getDuration());
        query.bindValue(":path", song.getPath());
        query.bindValue(":mp3", song.getMp3());
        query.bindValue(":lyric", song.getLyric());
        query.bindValue(":img", song.getImg());

        if (!query.exec()) {
            qDebug() << "错误存储歌曲到数据库：" << query.lastError().text();
        }
    }
}

QList<Song> MusicLibrary::getAllSongsFromDatabase() {
    QSqlQuery query("SELECT * FROM Songs");
    QList<Song> allSongs;

    while (query.next()) {
        QString musicName = query.value("MusicName").toString();
        QString albumName = query.value("AlbumName").toString();
        int duration = query.value("Duration").toInt();
        QString path = query.value("Path").toString();
        QString mp3 = query.value("MP3").toString();
        QString lyric = query.value("Lyric").toString();
        QString img = query.value("Image").toString();

        Song song(musicName, albumName, duration, path, mp3, lyric, img);
        allSongs.append(song);
    }

    return allSongs;
}

Song MusicLibrary::getCurrentSong() {
    return currentSong;
}
