#include "mainwindow.h"
#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Client client;
    client.createSocket();
    client.setAddrAndPort();
    client.connectServer();
    client.SendMsg();
    return a.exec();
}
