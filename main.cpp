#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDate>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ChatServer server;
    w.show();
    return a.exec();
}
