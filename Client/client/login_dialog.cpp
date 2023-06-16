#include "login_dialog.h"
#include <iostream>
#include <windows.h>
#include <QFuture>
#include <QFutureWatcher>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setFixedSize(335, 210);

    ipv4RadioBtn = new QRadioButton(this);
    ipv4RadioBtn->move(45, 20);
    ipv4RadioBtn->setText(tr("ipv4"));

    ipv6RadioBtn = new QRadioButton(this);
    ipv6RadioBtn->move(115, 20);
    ipv6RadioBtn->setText(tr("ipv6"));

    ipvSelect = new QButtonGroup(this);
    ipvSelect->addButton(ipv4RadioBtn, 0);
    ipvSelect->addButton(ipv6RadioBtn, 1);

    ipv4RadioBtn->setChecked(true);
    ipv6RadioBtn->setChecked(false);

    addrLabel = new QLabel(this);
    addrLabel->move(45, 60);
    addrLabel->setText(tr("ip address"));

    addrEditLine = new QLineEdit(this);
    addrEditLine->move(115, 60);
    addrEditLine->setPlaceholderText(tr("请输入要连接的服务器ip地址"));
    addrEditLine->setFixedSize(170, 20);
    addrEditLine->setText(tr("127.0.0.1"));

    portLabel = new QLabel(this);
    portLabel->move(45, 110);
    portLabel->setText(tr("port"));

    portEditLine = new QLineEdit(this);
    portEditLine->move(115, 110);
    portEditLine->setPlaceholderText(tr("请输入要连接的服务器端口号"));
    portEditLine->setFixedSize(170, 20);
    portEditLine->setText(tr("56789"));

    loginBtn = new QPushButton(this);
    loginBtn->move(45, 160);
    loginBtn->setText(tr("确认"));

    exitBtn = new QPushButton(this);
    exitBtn->move(210, 160);
    exitBtn->setText(tr("退出"));

    //信号与槽关联
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::login);
    connect(exitBtn, &QPushButton::clicked, this, &LoginDialog::close);
    connect(ipv4RadioBtn, &QRadioButton::clicked, this, &LoginDialog::setIpv4Default);
    connect(ipv6RadioBtn, &QRadioButton::clicked, this, &LoginDialog::setIpv6Default);
}

LoginDialog::~LoginDialog()
{
    delete addrLabel;
    delete addrEditLine;
    delete portLabel;
    delete portEditLine;
    delete loginBtn;
    delete exitBtn;
    delete ipv4RadioBtn;
    delete ipv6RadioBtn;
    delete ipvSelect;
    if (client != nullptr)
        delete client;
}

void LoginDialog::login()
{
    //判断用户名和密码是否正确（自动过滤首尾空格）
    client = nullptr;
    try
    {
        client = new Client(ipvSelect->checkedId());
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

void LoginDialog::setIpv4Default()
{
    addrEditLine->setText(tr("127.0.0.1"));
    portEditLine->setText(tr("56789"));
}

void LoginDialog::setIpv6Default()
{
    addrEditLine->setText(tr("2001:db8::1"));
    portEditLine->setText(tr("56789"));
}

