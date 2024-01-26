#ifndef __LOCALCACHE_H__
#define __LOCALCACHE_H__

// Model - ���ػ������

#include "common.h" // �������ڱ�ʾ������ͷ�ļ�

class LocalCache : public QObject {
    Q_OBJECT

public:
    void downloadFile(const QString& fileUrl, const QString& outputPath);
    // �ӱ��ػ�����ɾ������
    void deleteCachedMusic(const Song& song);
    // ���������浽����
    void cacheMusic(const Song& song); 
    
signals:
    void addLocalMusic(const Song& localSong);// ���±��������б�

private:
    // ���ػ���洢��·��
    QString cacheFolderPath = "./localcache";
};

#endif // __LOCALCACHE_H__
