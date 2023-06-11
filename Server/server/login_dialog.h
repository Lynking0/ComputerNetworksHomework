#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "server.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    Server *server;

    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

//槽函数
private slots:
    void login();

private:
    QLabel *addrLabel;
    QLabel *portLabel;

    QLabel *waitingLabel;

    QLineEdit *addrEditLine;
    QLineEdit *portEditLine;

    QPushButton *loginBtn;
    QPushButton *exitBtn;
};

#endif // LOGINDIALOG_H
