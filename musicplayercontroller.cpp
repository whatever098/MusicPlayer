#include "musicplayercontroller.h"

QPixmap PixmapToRound(const QPixmap& src, int radius)// �ü�ͼƬΪԲ��
{
    if (src.isNull()){
        return QPixmap();
    }

    //����������
    QPixmap pixmapa;
    if (src.width() != radius || src.height() != radius){
        pixmapa = src.scaled(radius, radius, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else{
        pixmapa = src;
    }

    QPixmap pixmap(radius, radius);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.addEllipse(0, 0, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, radius, radius, pixmapa);

    return pixmap;
}

MusicPlayerController::MusicPlayerController(MusicEngine* eng, LocalCache* cach,
    MusicPlayerUI* v, MusicListWidget* ml)
    : engine(eng), cache(cach), view(v), musicList(ml),timer(new QTimer)
{
    // ���� view ���е��źź� model ���еĲۺ���
    connect(timer, &QTimer::timeout, this, &MusicPlayerController::handleUpdateLyrics);
    connect(timer, &QTimer::timeout, this, &MusicPlayerController::handleUpdatePrograss);
    connect(view, &MusicPlayerUI::playClicked, this, &MusicPlayerController::handlePlayButtonClicked);
    connect(view, &MusicPlayerUI::nextClicked, this, &MusicPlayerController::handleNextButtonClicked);
    connect(view, &MusicPlayerUI::previousClicked, this, &MusicPlayerController::handlePreviousButtonClicked);
    connect(view, &MusicPlayerUI::sliderMoved, this, &MusicPlayerController::handleSliderMoved);
    connect(view, &MusicPlayerUI::volumeChanged, this, &MusicPlayerController::handleVolumeChanged);
    connect(view, &MusicPlayerUI::modeChanged, this, &MusicPlayerController::handleModeChanged);
    connect(lib, &MusicLibrary::onlineMusicListReady, this, &MusicPlayerController::handleOnlineMusicList);
    connect(lib, &MusicLibrary::localMusicListReady, this, &MusicPlayerController::handleLocalMusicList);
    connect(musicList, &MusicListWidget::localSongSelected, this, &MusicPlayerController::handleLocalPlaySong);
    connect(musicList, &MusicListWidget::onlineSongSelected, this, &MusicPlayerController::handleOnlinePlaySong);
    connect(musicList, &MusicListWidget::searchOnline, this, &MusicPlayerController::handleSearchOnline);
    connect(engine, &MusicEngine::songChanged, this, &MusicPlayerController::handleSongChanged);
    connect(cache, &LocalCache::addLocalMusic, this, &MusicPlayerController::handleAddLocalMusic);
    timer->start(1000);
    lib->fetchLocalMusic("./localcache");
}

void MusicPlayerController::handleAddLocalMusic(const Song& localSong){
    musicList->addLocalMusic(localSong);
}

void MusicPlayerController::handlePlayButtonClicked() 
{
    if (engine->isPaused()) {
        view->getUi()->playButton->setIcon(QIcon("./images/pause.png"));
        engine->play(); 
    }
    else {
        view->getUi()->playButton->setIcon(QIcon("./images/player.png"));
        engine->pause();
    }
}

void MusicPlayerController::showImage(const Song& selectedSong) {
    // Ĭ��ͼƬ·��
    QString defaultImagePath = "./images/none.jpg";

    QString path1 = selectedSong.getPath();
    if (path1[0] != '.') path1 = "./localcache" + path1;
    // ����ԭʼͼƬ
    QPixmap originalPixmap(path1 + selectedSong.getImg());

    // ���ԭʼͼƬ�Ƿ���Ч
    if (originalPixmap.isNull()) {
        // ���ԭʼͼƬ��Ч����ʹ��Ĭ��ͼƬ
        originalPixmap = QPixmap(defaultImagePath);
    }

    // ����ͼƬ���ʵ���С
    QSize targetSize(300, 300); // �����ʵ��Ĵ�С
    QPixmap scaledPixmap = originalPixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // ��ͼƬ�ü�ΪԲ��
    QPixmap roundedPixmap = PixmapToRound(scaledPixmap, 200); // ����Բ�ǰ뾶

    // ���� QGraphicsView �� QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene;
    view->getUi()->albumImageView->setScene(scene);
    view->getUi()->albumImageView->setFixedSize(targetSize);

    // ��Բ��ͼƬ��ӵ� QGraphicsScene ��
    QGraphicsPixmapItem* pixmapItem = scene->addPixmap(roundedPixmap);

    // ��ͼƬ������� QGraphicsView ���Ĳ�������ת����ΪͼƬ����
    pixmapItem->setPos((targetSize.width() - roundedPixmap.width()) / 2, (targetSize.height() - roundedPixmap.height()) / 2);
    pixmapItem->setTransformOriginPoint(roundedPixmap.width() / 2, roundedPixmap.height() / 2);

    // ������ʱ���������ӵ��ۺ����Գ���������ת�Ƕ�
    QTimer* timer1 = new QTimer(this);
    QObject::connect(timer1, &QTimer::timeout, [=]() {
        static qreal rotationAngle = 0.0;
        rotationAngle += 0.1; // ÿ��������ת�Ƕ�

        // ����ת�Ƕ�Ӧ���� QGraphicsPixmapItem
        pixmapItem->setRotation(rotationAngle);
        });
    timer1->start(30);
}


void MusicPlayerController::handleLocalPlaySong(const Song& selectedSong)
{
    view->getUi()->playButton->setIcon(QIcon("./images/pause.png"));
    view->getLysic()->processLyricsFile(selectedSong.getPath() + selectedSong.getLyric());

    showImage(selectedSong);// ר����ת

    QString songInfo = QString::fromUtf8("Artist-Music: %1\nAlbumName: %2\n")
        .arg(selectedSong.getMusicName())
        .arg(selectedSong.getAlbumName());

    view->getUi()->songInfoLable->setText(songInfo);

    engine->playSong(selectedSong);

    QListWidget* currentListWidget = view->getUi()->current_list;
    QString songInfo1 = selectedSong.getMusicName() + " - " + selectedSong.getAlbumName();
    bool itemExists = false;
    for (int i = 0; i < currentListWidget->count(); ++i) {
        QListWidgetItem* existingItem = currentListWidget->item(i);
        if (existingItem && existingItem->text() == songInfo1) {
            itemExists = true;
            break;
        }
    }
    if (!itemExists) {
        QListWidgetItem* newItem = new QListWidgetItem(songInfo1);
        currentListWidget->addItem(newItem);
    }
}

void MusicPlayerController::handleOnlinePlaySong(const Song& selectedSong) {
    cache->cacheMusic(selectedSong);
    handleLocalPlaySong(selectedSong);
}

void MusicPlayerController::handleSongChanged(Song& song) {
    view->getLysic()->processLyricsFile(song.getPath() + song.getLyric());// ���¸��
    QString songInfo = QString::fromUtf8("Artist-Music: %1\nAlbumName: %2\n")
        .arg(song.getMusicName())
        .arg(song.getAlbumName()); 

    showImage(song);// ר������
    view->getUi()->songInfoLable->setText(songInfo);
}

void MusicPlayerController::handleNextButtonClicked() {
    engine->nextSong(); // ������һ�׸���
}

void MusicPlayerController::handlePreviousButtonClicked() {
    engine->previousSong(); // ������һ�׸���
}

void MusicPlayerController::handleSliderMoved(qint64 position) {
    position = position * engine->duration() / 100;
    engine->setPosition(position); // ���ò��Ž�����λ��
}

void MusicPlayerController::handleVolumeChanged(qint64 volume) {
    engine->setVolume(volume); // ��������
}

void MusicPlayerController::handleUpdateLyrics() {
    view->getLysic()->updateLyrics(engine->position());
}

void MusicPlayerController::handleSearchOnline(const QString& folderName) {
    lib->fetchOnlineMusic(folderName);
}

void MusicPlayerController::handleUpdatePrograss() {
    qint64 duration = engine->duration();
    int totalMinutes = duration / 60000; // ��ȡ�ܷ�����
    int totalSeconds = (duration / 1000) % 60; // ��ȡʣ�������

    QString totalDurationString = QString("%1:%2")
        .arg(totalMinutes, 2, 10, QLatin1Char('0')) // 2 ��ʾ��ȣ��� '0' ���
        .arg(totalSeconds, 2, 10, QLatin1Char('0')); // ����ʱ����ʽ��Ϊ "00:00" ��ʽ

    view->getUi()->total_time->setText(totalDurationString); // ����ʽ�������ʱ����ʾ�� UI ��

    // ��ȡ��ǰ���ŵ�λ��
    qint64 position = engine->position();
    int currentMinutes = position / 60000; // ��ȡ��ǰ���ŵķ�����
    int currentSeconds = (position / 1000) % 60; // ��ȡʣ�������

    QString currentPositionString = QString("%1:%2")
        .arg(currentMinutes, 2, 10, QLatin1Char('0')) // 2 ��ʾ��ȣ��� '0' ���
        .arg(currentSeconds, 2, 10, QLatin1Char('0')); // ����ǰ����λ�ø�ʽ��Ϊ "00:00" ��ʽ

    view->getUi()->current_time->setText(currentPositionString); // ����ʽ����ĵ�ǰ����λ����ʾ�� UI ��

    // ������������
    if (duration > 0) {
        int progress = static_cast<int>((position * 100) / duration); // ���㲥�Ž���
        view->getUi()->positionSlider->setValue(progress); // ���������õ���������
    }
}

void MusicPlayerController::handleModeChanged() {
    engine->setPlayMode(); // ���ò���ģʽ
    switch (engine->getCurrentPlayMode()) {
    case PlayMode::Sequential:
        view->getUi()->playModeButton->setIcon(QIcon("./images/sequential.png"));
        break;
    case PlayMode::Loop:
        view->getUi()->playModeButton->setIcon(QIcon("./images/loop.png"));
        break;
    case PlayMode::Random:
        view->getUi()->playModeButton->setIcon(QIcon("./images/random.png"));
        break;
    case PlayMode::RepeatOne:
        view->getUi()->playModeButton->setIcon(QIcon("./images/repeatone.png"));
        break;
    default:
        break;
    }
}

void MusicPlayerController::handleLocalMusicList(QList<Song> localMusicList)
{
    musicList->showLocalMusicList(localMusicList);// ��ͼ����ʾ���������б�
}

void MusicPlayerController::handleOnlineMusicList(QList<Song> onlineMusicList)
{
    musicList->showOnlineMusicList(onlineMusicList);// ��ͼ����ʾ���������б�
}
