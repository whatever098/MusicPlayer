#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

// view - 音乐列表视图

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
    // 处理音乐列表中的歌曲项点击
    void localMusicItemClicked(QListWidgetItem* item);
    void onlineMusicItemClicked(QListWidgetItem* item);
    void onSearchButtonClicked();
   
signals:
    // 发送选定的歌曲
    void localSongSelected(const Song& selectedSong);
    void onlineSongSelected(const Song& selectedSong);
    void searchOnline(const QString& folderName);

private:
    Ui::MusicList* ui;

    // 填充音乐列表部件
    void populateListWidget(QListWidget* listWidget, const QList<Song>& songs);
};

#endif // MUSICLISTWIDGET_H
