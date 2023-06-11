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
    void handleWriteConsole(const QString &msg);

private:
    QFutureWatcher<void> *pwatcher;

    QLineEdit *msgEditLine;
    QPushButton *sendBtn;
    QPushButton *clearBtn;
    Console *console;
};
#endif // MAINWINDOW_H
