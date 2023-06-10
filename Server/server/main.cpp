#include "mainwindow.h"
#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Server server;
    server.createSocket();
    server.setAddrAndPort();
    server.bindSocket();
    server.startListen();
    server.waitClient();
    server.receiveMsg();
    return a.exec();
}
