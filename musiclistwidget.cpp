#include "MusicListWidget.h"

MusicListWidget::MusicListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MusicList) 
{
    ui->setupUi(this);
    // 连接音乐列表项点击事件
    connect(ui->listWidget_2, &QListWidget::itemClicked, this, &MusicListWidget::localMusicItemClicked);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MusicListWidget::onlineMusicItemClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MusicListWidget::onSearchButtonClicked);
}

MusicListWidget::~MusicListWidget() 
{
    delete ui;
}

void MusicListWidget::showLocalMusicList(QList<Song>& localMusicList) {
    // 显示本地音乐列表
    populateListWidget(ui->listWidget_2, localMusicList);
}

void MusicListWidget::showOnlineMusicList(QList<Song>& onlineMusicList) {
    // 显示线上音乐列表
    populateListWidget(ui->listWidget, onlineMusicList);
}

void MusicListWidget::addLocalMusic(const Song& localSong) {
    // 更新本地音乐列表
    QString displayText = QString("%1 - %2").arg(localSong.getMusicName()).arg(localSong.getAlbumName());
    auto item = new QListWidgetItem(displayText);
    SongItemData songData;
    songData.song = localSong;
    songData.displayText = displayText;
    item->setData(Qt::UserRole, QVariant::fromValue(songData));
    ui->listWidget_2->addItem(item);
    ui->listWidget_2->repaint();
}

void MusicListWidget::onSearchButtonClicked() {
    QString searchText = ui->lineEdit->text(); // 获取文本框中的搜索文本
    emit searchOnline(searchText);
}

void MusicListWidget::localMusicItemClicked(QListWidgetItem* item) {
    // 发送所选歌曲信号
    emit localSongSelected(item->data(Qt::UserRole).value<SongItemData>().song);
}

void MusicListWidget::onlineMusicItemClicked(QListWidgetItem* item) {
    // 发送所选歌曲信号
    emit onlineSongSelected(item->data(Qt::UserRole).value<SongItemData>().song);
}

void MusicListWidget::populateListWidget(QListWidget* listWidget, const QList<Song>& songs) {
    listWidget->clear();

    // 为音乐列表部件填充数据
    for (const Song& song : songs) {
        QString displayText = QString("%1 - %2").arg(song.getMusicName()).arg(song.getAlbumName());
        auto item = new QListWidgetItem(displayText);
        SongItemData songData;
        songData.song = song;
        songData.displayText = displayText;
        item->setData(Qt::UserRole, QVariant::fromValue(songData));
        listWidget->addItem(item);
    }

    listWidget->repaint();
}

