#include <QtWidgets/QApplication>
#include "musicplayercontroller.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    MainWindow w; // 创建一个 MainWindow 对象
    w.setWindowIcon(QIcon("./images/client.png"));
    w.setFixedSize(1000, 800); // 设置窗口的固定大小
    w.show(); // 显示 MainWindow
    return a.exec();
}
