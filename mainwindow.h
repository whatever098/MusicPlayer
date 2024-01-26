#include "common.h"
#include "musicplayercontroller.h"
#include "x64/Debug/uic/ui_mainwindow.h"

// view - ������


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
    Ui::MainWindow* ui;  // ���� UI ָ��
    QButtonGroup btnGroup; // ���尴ť��
    QSystemTrayIcon* trayIcon;

    void createTrayIcon();
    void setupUI();
};

