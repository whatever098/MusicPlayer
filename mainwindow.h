#include "common.h"
#include "musicplayercontroller.h"
#include "x64/Debug/uic/ui_mainwindow.h"

// view - 主窗口


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override {
        if (trayIcon->isVisible()) {
            hide();
            event->ignore();
        }
        else {
            QMainWindow::closeEvent(event);
        }
    }

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            showNormal();
        }
    }

private:
    Ui::MainWindow* ui;  // 定义 UI 指针
    QButtonGroup btnGroup; // 定义按钮组
    QSystemTrayIcon* trayIcon;

    void createTrayIcon();
    void setupUI();
};

