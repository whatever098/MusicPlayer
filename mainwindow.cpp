#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
    createTrayIcon();
}

void MainWindow::createTrayIcon() {
    trayIcon = new QSystemTrayIcon(QIcon("./images/client.png"), this);
    trayIcon->setToolTip("My Music Player");

    QMenu* trayMenu = new QMenu(this);
    QAction* restoreAction = new QAction("Restore", this);
    QAction* quitAction = new QAction("Quit", this);

    connect(restoreAction, &QAction::triggered, this, &MainWindow::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    trayMenu->addAction(restoreAction);
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUI() {
    MusicLibrary* lib = MusicLibrary::getInstance();
    MusicPlayerUI* playingPage = new MusicPlayerUI;
    MusicListWidget* MusicPage = new MusicListWidget;
    MusicEngine* engine = new MusicEngine();
    LocalCache* cache = new LocalCache;

    MusicPlayerController* Player = new MusicPlayerController(engine, cache, playingPage, MusicPage);

    ui->stackedWidget->addWidget(playingPage);
    ui->stackedWidget->addWidget(MusicPage);

    btnGroup.addButton(ui->toolButton, 0);
    btnGroup.addButton(ui->toolButton_2, 1);
    connect(&btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    // 设置默认选中的页面
    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);

    setWindowTitle("Music Player");
}