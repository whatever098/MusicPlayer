#include "MusicListWidget.h"

MusicListWidget::MusicListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MusicList) 
{
    ui->setupUi(this);
    // ���������б������¼�
    connect(ui->listWidget_2, &QListWidget::itemClicked, this, &MusicListWidget::localMusicItemClicked);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MusicListWidget::onlineMusicItemClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MusicListWidget::onSearchButtonClicked);
}

MusicListWidget::~MusicListWidget() 
{
    delete ui;
}

void MusicListWidget::showLocalMusicList(QList<Song>& localMusicList) {
    // ��ʾ���������б�
    populateListWidget(ui->listWidget_2, localMusicList);
}

void MusicListWidget::showOnlineMusicList(QList<Song>& onlineMusicList) {
    // ��ʾ���������б�
    populateListWidget(ui->listWidget, onlineMusicList);
}

void MusicListWidget::addLocalMusic(const Song& localSong) {
    // ���±��������б�
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
    QString searchText = ui->lineEdit->text(); // ��ȡ�ı����е������ı�
    emit searchOnline(searchText);
}

void MusicListWidget::localMusicItemClicked(QListWidgetItem* item) {
    // ������ѡ�����ź�
    emit localSongSelected(item->data(Qt::UserRole).value<SongItemData>().song);
}

void MusicListWidget::onlineMusicItemClicked(QListWidgetItem* item) {
    // ������ѡ�����ź�
    emit onlineSongSelected(item->data(Qt::UserRole).value<SongItemData>().song);
}

void MusicListWidget::populateListWidget(QListWidget* listWidget, const QList<Song>& songs) {
    listWidget->clear();

    // Ϊ�����б����������
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

