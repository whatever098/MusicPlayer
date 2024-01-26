#ifndef __LOCALCACHE_H__
#define __LOCALCACHE_H__

// Model - 本地缓存管理

#include "common.h" // 包含用于表示歌曲的头文件

class LocalCache : public QObject {
    Q_OBJECT

public:
    void downloadFile(const QString& fileUrl, const QString& outputPath);
    // 从本地缓存中删除歌曲
    void deleteCachedMusic(const Song& song);
    // 将歌曲缓存到本地
    void cacheMusic(const Song& song); 
    
signals:
    void addLocalMusic(const Song& localSong);// 更新本地音乐列表

private:
    // 本地缓存存储的路径
    QString cacheFolderPath = "./localcache";
};

#endif // __LOCALCACHE_H__
