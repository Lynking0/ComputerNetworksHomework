#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "console.h"
#include "client.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Client *client;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMsg();
    void clearConsole();

private:
    QLineEdit *msgEditLine;
    QPushButton *sendBtn;
    QPushButton *clearBtn;
    Console *console;
};
#endif // MAINWINDOW_H
