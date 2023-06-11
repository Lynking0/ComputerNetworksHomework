#include "login_dialog.h"
#include <iostream>
#include <windows.h>
#include <QFuture>
#include <QFutureWatcher>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setFixedSize(335, 180);

    addrLabel = new QLabel(this);
    addrLabel->move(45, 30);
    addrLabel->setText(tr("ip address"));

    addrEditLine = new QLineEdit(this);
    addrEditLine->move(115, 30);
    addrEditLine->setPlaceholderText(tr("请输入要连接的服务器ip地址"));
    addrEditLine->setFixedSize(170, 20);
    addrEditLine->setText(tr("127.0.0.1"));

    portLabel = new QLabel(this);
    portLabel->move(45, 80);
    portLabel->setText(tr("port"));

    portEditLine = new QLineEdit(this);
    portEditLine->move(115, 80);
    portEditLine->setPlaceholderText(tr("请输入要连接的服务器端口号"));
    portEditLine->setFixedSize(170, 20);
    portEditLine->setText(tr("56789"));

    loginBtn = new QPushButton(this);
    loginBtn->move(45, 130);
    loginBtn->setText(tr("确认"));

    exitBtn = new QPushButton(this);
    exitBtn->move(210, 130);
    exitBtn->setText(tr("退出"));

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
    if (client != nullptr)
        delete client;
}

void LoginDialog::login()
{
    //判断用户名和密码是否正确（自动过滤首尾空格）
    client = nullptr;
    try
    {
        client = new Client();
        client->createSocket();
        client->setAddrAndPort(addrEditLine->text().trimmed().toLatin1().data(), portEditLine->text().trimmed().toUShort());
        client->connectServer();
        accept();
    }
    catch(const std::runtime_error &e)
    {
        if (client != nullptr)
            delete client;
        client = nullptr;
        QMessageBox::warning(this, tr("警告"), tr(e.what()), QMessageBox::Yes);

        // 清空输入框并设置焦点
        addrEditLine->clear();
        portEditLine->clear();
        addrEditLine->setFocus();
    }
}
