#include "mainwindow.h"
#include "client.h"
#include "login_dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    LoginDialog loginDlg;

    if (loginDlg.exec() == QDialog::Accepted)
    {
        w.client = loginDlg.client;
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
