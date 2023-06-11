#include "login_dialog.h"
#include <iostream>
#include <windows.h>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setFixedSize(335, 180);

    addrLabel = new QLabel(this);
    addrLabel->move(50, 30);
    addrLabel->setText(tr("ip address"));

    addrEditLine = new QLineEdit(this);
    addrEditLine->move(130, 30);
    addrEditLine->setPlaceholderText(tr("请输入服务器ip地址"));
    addrEditLine->setFixedSize(150, 20);
    addrEditLine->setText(tr("127.0.0.1"));

    portLabel = new QLabel(this);
    portLabel->move(50, 80);
    portLabel->setText(tr("port"));

    portEditLine = new QLineEdit(this);
    portEditLine->move(130, 80);
    portEditLine->setPlaceholderText(tr("请输入服务器端口号"));
    portEditLine->setFixedSize(150, 20);
    portEditLine->setText(tr("56789"));

    loginBtn = new QPushButton(this);
    loginBtn->move(50, 130);
    loginBtn->setText(tr("确认"));

    exitBtn = new QPushButton(this);
    exitBtn->move(205, 130);
    exitBtn->setText(tr("退出"));

    waitingLabel = new QLabel(this);
    waitingLabel->move(5, 0);
    waitingLabel->hide();

    //信号与槽关联
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::login);
    connect(exitBtn, &QPushButton::clicked, this, &LoginDialog::close);
}

LoginDialog::~LoginDialog()
{
    delete addrLabel;
    delete addrEditLine;
    delete portLabel;
    delete portEditLine;
    delete loginBtn;
    delete exitBtn;
    if (server != nullptr)
        delete server;
}

void LoginDialog::login()
{
    //判断用户名和密码是否正确（自动过滤首尾空格）
    server = nullptr;
    try
    {
        server = new Server();
        server->createSocket();
        server->setAddrAndPort(addrEditLine->text().trimmed().toLatin1().data(), portEditLine->text().trimmed().toUShort());
        server->bindSocket();
        server->startListen();

        addrLabel->hide();
        addrEditLine->hide();
        portLabel->hide();
        portEditLine->hide();
        loginBtn->hide();
        exitBtn->hide();
        waitingLabel->setText(QString("ip address: ") + addrEditLine->text().trimmed() + QString('\n') +
                              QString("port: ") + portEditLine->text().trimmed() + QString('\n') +
                              tr("正在等待客户端的连接请求..."));
        waitingLabel->show();

        // 异步
        QFutureWatcher<void> *pwatcher = new QFutureWatcher<void>;

        QFuture<void> future = QtConcurrent::run([=]() {
            server->waitClient();
        });

        connect(pwatcher, &QFutureWatcher<void>::finished, this, [=]() {
            accept();
        });

        pwatcher->setFuture(future);
    }
    catch(const std::runtime_error &e)
    {
        if (server != nullptr)
            delete server;
        server = nullptr;
        QMessageBox::warning(this, tr("警告"), tr(e.what()), QMessageBox::Yes);

        // 还原界面
        addrLabel->show();
        addrEditLine->show();
        portLabel->show();
        portEditLine->show();
        loginBtn->show();
        exitBtn->show();
        waitingLabel->hide();

        // 清空输入框并设置焦点
        addrEditLine->clear();
        portEditLine->clear();
        addrEditLine->setFocus();
    }
}
