#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

// view - �����б���ͼ

#include "common.h"
#include "./x64/Debug/uic/ui_musiclist.h"

class MusicListWidget : public QWidget {
    Q_OBJECT

public:
    explicit MusicListWidget(QWidget* parent = nullptr);
    ~MusicListWidget();

    void showLocalMusicList(QList<Song>& localMusicList);
    void showOnlineMusicList(QList<Song>& onlineMusicList);
    void addLocalMusic(const Song& localSong);

public slots:
    // ���������б��еĸ�������
    void localMusicItemClicked(QListWidgetItem* item);
    void onlineMusicItemClicked(QListWidgetItem* item);
    void onSearchButtonClicked();
   
signals:
    // ����ѡ���ĸ���
    void localSongSelected(const Song& selectedSong);
    void onlineSongSelected(const Song& selectedSong);
    void searchOnline(const QString& folderName);

private:
    Ui::MusicList* ui;

    // ��������б���
    void populateListWidget(QListWidget* listWidget, const QList<Song>& songs);
};

#endif // MUSICLISTWIDGET_H
