#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "client.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    Client *client;

    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

//槽函数
private slots:
    void login();
    void setIpv4Default();
    void setIpv6Default();

private:
    QButtonGroup *ipvSelect;
    QRadioButton *ipv4RadioBtn;
    QRadioButton *ipv6RadioBtn;

    QLabel *addrLabel;
    QLabel *portLabel;

    QLineEdit *addrEditLine;
    QLineEdit *portEditLine;

    QPushButton *loginBtn;
    QPushButton *exitBtn;
};

#endif // LOGINDIALOG_H
