#include "musicplayercontroller.h"

QPixmap PixmapToRound(const QPixmap& src, int radius)// 裁剪图片为圆形
{
    if (src.isNull()){
        return QPixmap();
    }

    //按比例缩放
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
    // 连接 view 层中的信号和 model 层中的槽函数
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
    // 默认图片路径
    QString defaultImagePath = "./images/none.jpg";

    QString path1 = selectedSong.getPath();
    if (path1[0] != '.') path1 = "./localcache" + path1;
    // 加载原始图片
    QPixmap originalPixmap(path1 + selectedSong.getImg());

    // 检查原始图片是否有效
    if (originalPixmap.isNull()) {
        // 如果原始图片无效，则使用默认图片
        originalPixmap = QPixmap(defaultImagePath);
    }

    // 缩放图片到适当大小
    QSize targetSize(300, 300); // 设置适当的大小
    QPixmap scaledPixmap = originalPixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 将图片裁剪为圆形
    QPixmap roundedPixmap = PixmapToRound(scaledPixmap, 200); // 设置圆角半径

    // 创建 QGraphicsView 和 QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene;
    view->getUi()->albumImageView->setScene(scene);
    view->getUi()->albumImageView->setFixedSize(targetSize);

    // 将圆形图片添加到 QGraphicsScene 中
    QGraphicsPixmapItem* pixmapItem = scene->addPixmap(roundedPixmap);

    // 将图片项放置在 QGraphicsView 中心并设置旋转中心为图片中心
    pixmapItem->setPos((targetSize.width() - roundedPixmap.width()) / 2, (targetSize.height() - roundedPixmap.height()) / 2);
    pixmapItem->setTransformOriginPoint(roundedPixmap.width() / 2, roundedPixmap.height() / 2);

    // 创建定时器，并连接到槽函数以持续更新旋转角度
    QTimer* timer1 = new QTimer(this);
    QObject::connect(timer1, &QTimer::timeout, [=]() {
        static qreal rotationAngle = 0.0;
        rotationAngle += 0.1; // 每次增加旋转角度

        // 将旋转角度应用于 QGraphicsPixmapItem
        pixmapItem->setRotation(rotationAngle);
        });
    timer1->start(30);
}


void MusicPlayerController::handleLocalPlaySong(const Song& selectedSong)
{
    view->getUi()->playButton->setIcon(QIcon("./images/pause.png"));
    view->getLysic()->processLyricsFile(selectedSong.getPath() + selectedSong.getLyric());

    showImage(selectedSong);// 专辑旋转

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
    view->getLysic()->processLyricsFile(song.getPath() + song.getLyric());// 更新歌词
    QString songInfo = QString::fromUtf8("Artist-Music: %1\nAlbumName: %2\n")
        .arg(song.getMusicName())
        .arg(song.getAlbumName()); 

    showImage(song);// 专辑更新
    view->getUi()->songInfoLable->setText(songInfo);
}

void MusicPlayerController::handleNextButtonClicked() {
    engine->nextSong(); // 播放下一首歌曲
}

void MusicPlayerController::handlePreviousButtonClicked() {
    engine->previousSong(); // 播放上一首歌曲
}

void MusicPlayerController::handleSliderMoved(qint64 position) {
    position = position * engine->duration() / 100;
    engine->setPosition(position); // 设置播放进度条位置
}

void MusicPlayerController::handleVolumeChanged(qint64 volume) {
    engine->setVolume(volume); // 设置音量
}

void MusicPlayerController::handleUpdateLyrics() {
    view->getLysic()->updateLyrics(engine->position());
}

void MusicPlayerController::handleSearchOnline(const QString& folderName) {
    lib->fetchOnlineMusic(folderName);
}

void MusicPlayerController::handleUpdatePrograss() {
    qint64 duration = engine->duration();
    int totalMinutes = duration / 60000; // 获取总分钟数
    int totalSeconds = (duration / 1000) % 60; // 获取剩余的秒数

    QString totalDurationString = QString("%1:%2")
        .arg(totalMinutes, 2, 10, QLatin1Char('0')) // 2 表示宽度，用 '0' 填充
        .arg(totalSeconds, 2, 10, QLatin1Char('0')); // 将总时长格式化为 "00:00" 形式

    view->getUi()->total_time->setText(totalDurationString); // 将格式化后的总时长显示到 UI 中

    // 获取当前播放的位置
    qint64 position = engine->position();
    int currentMinutes = position / 60000; // 获取当前播放的分钟数
    int currentSeconds = (position / 1000) % 60; // 获取剩余的秒数

    QString currentPositionString = QString("%1:%2")
        .arg(currentMinutes, 2, 10, QLatin1Char('0')) // 2 表示宽度，用 '0' 填充
        .arg(currentSeconds, 2, 10, QLatin1Char('0')); // 将当前播放位置格式化为 "00:00" 形式

    view->getUi()->current_time->setText(currentPositionString); // 将格式化后的当前播放位置显示到 UI 中

    // 避免除以零错误
    if (duration > 0) {
        int progress = static_cast<int>((position * 100) / duration); // 计算播放进度
        view->getUi()->positionSlider->setValue(progress); // 将进度设置到进度条上
    }
}

void MusicPlayerController::handleModeChanged() {
    engine->setPlayMode(); // 设置播放模式
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
    musicList->showLocalMusicList(localMusicList);// 视图层显示本地音乐列表
}

void MusicPlayerController::handleOnlineMusicList(QList<Song> onlineMusicList)
{
    musicList->showOnlineMusicList(onlineMusicList);// 视图层显示在线音乐列表
}
