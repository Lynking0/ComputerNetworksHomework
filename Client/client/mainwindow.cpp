#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(500, 650);

    msgEditLine = new QLineEdit(this);
    msgEditLine->move(30, 20);
    msgEditLine->setPlaceholderText(tr("请输入要发送至服务器端的消息"));
    msgEditLine->setFixedSize(440, 25);

    sendBtn = new QPushButton(this);
    sendBtn->move(30, 55);
    sendBtn->setText(tr("发送消息"));

    clearBtn = new QPushButton(this);
    clearBtn->move(155, 55);
    clearBtn->setText(tr("清空控制台"));

    console = new Console(this);
    console->move(30, 100);
    console->setFixedSize(440, 530);

    //信号与槽关联
    connect(sendBtn, &QPushButton::clicked, this, &MainWindow::sendMsg);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearConsole);
}

MainWindow::~MainWindow()
{
    delete msgEditLine;
    delete sendBtn;
    delete clearBtn;
    delete console;
}

void MainWindow::sendMsg()
{
    try
    {
        QString msg = msgEditLine->text().trimmed();
        if (msg.length() != 0)
            client->SendMsg(msg.toLatin1().data(), msg.length());
    }
    catch(const std::runtime_error &e)
    {
        console->write(e.what());
    }
    msgEditLine->setText("");
}

void MainWindow::clearConsole()
{
    console->clear();
}

