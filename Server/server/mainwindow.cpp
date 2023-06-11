#include "mainwindow.h"
#include <QMetaObject>
#include <cstring>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    server = nullptr;

    setFixedSize(500, 650);

    msgEditLine = new QLineEdit(this);
    msgEditLine->move(30, 20);
    msgEditLine->setPlaceholderText(tr("请输入要发送至客户端的消息"));
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
    connect(this, &MainWindow::writeConsole, this, &MainWindow::handleWriteConsole);

    pwatcher = new QFutureWatcher<void>;
    QFuture<void> future = QtConcurrent::run([=]() {
        QString msg;
        while (true)
        {
            if (server == nullptr)
                continue;
            try
            {
                bool isEnd;
                char *str = server->receiveMsg(isEnd);
                if (str != nullptr)
                {
                    msg += str;
                    if (isEnd)
                    {
                        QMetaObject::invokeMethod(this, "writeConsole", Qt::QueuedConnection,
                                                  Q_ARG(QString, QString("receive message from client: ") + msg));
                        msg.clear();
                    }
                    delete[] str;
                }
            }
            catch (const std::runtime_error &e)
            {
                QMetaObject::invokeMethod(this, "writeConsole", Qt::QueuedConnection,
                                          Q_ARG(QString, e.what()));
                break;
            }
        }
    });
    pwatcher->setFuture(future);
}

MainWindow::~MainWindow()
{
    delete msgEditLine;
    delete sendBtn;
    delete clearBtn;
    delete console;
    delete pwatcher;
}

void MainWindow::sendMsg()
{
    msgEditLine->setText("");
}

void MainWindow::clearConsole()
{
    console->clear();
}

void MainWindow::handleWriteConsole(const QString &msg)
{
    console->write(msg + '\n');
}
