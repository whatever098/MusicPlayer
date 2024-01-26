#include <QtWidgets/QApplication>
#include "musicplayercontroller.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    MainWindow w; // ����һ�� MainWindow ����
    w.setWindowIcon(QIcon("./images/client.png"));
    w.setFixedSize(1000, 800); // ���ô��ڵĹ̶���С
    w.show(); // ��ʾ MainWindow
    return a.exec();
}
