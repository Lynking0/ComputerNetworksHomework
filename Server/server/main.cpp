#include "mainwindow.h"
#include "server.h"
#include "login_dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    LoginDialog loginDlg;

    if (loginDlg.exec() == QDialog::Accepted)
    {
        w.server = loginDlg.server;
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
