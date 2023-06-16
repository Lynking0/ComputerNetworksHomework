#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include "console.h"
#include "server.h"
#include "package_manager.h"

const QString MSG_FLAG = QString(char(1));
const QString FILE_FLAG = QString(char(2));
const int PACKAGE_SIZE = (32 * 1024 - 1 - 1 - 4 - 4 - 4 - 64);

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void writeConsole(const QString &msg);

public:
    Server *server;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMsg();
    void clearConsole();
    void selectFile();
    void handleWriteConsole(const QString &msg);
    void handleReceiveFinish(const QString &fileName, const QByteArray &content);

private:
    PackageManager *packageManager;
    QFutureWatcher<void> *pwatcher;

    QLineEdit *msgEditLine;
    QPushButton *sendBtn;
    QPushButton *clearBtn;
    QPushButton *selectBtn;
    Console *console;
};
#endif // MAINWINDOW_H
