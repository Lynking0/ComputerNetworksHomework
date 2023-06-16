#include "mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <ctime>
#include <QTextCodec>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    client = nullptr;
    packageManager = new PackageManager();

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

    selectBtn = new QPushButton(this);
    selectBtn->move(280, 55);
    selectBtn->setText(tr("选择文件"));

    console = new Console(this);
    console->move(30, 100);
    console->setFixedSize(440, 530);

    //信号与槽关联
    connect(sendBtn, &QPushButton::clicked, this, &MainWindow::sendMsg);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearConsole);
    connect(selectBtn, &QPushButton::clicked, this, &MainWindow::selectFile);
    connect(this, &MainWindow::writeConsole, this, &MainWindow::handleWriteConsole);
    connect(packageManager, &PackageManager::receiveFinish, this, &MainWindow::handleReceiveFinish);

    pwatcher = new QFutureWatcher<void>;
    QFuture<void> future = QtConcurrent::run([=]() {
        while (true)
        {
            if (client == nullptr)
                continue;
            try
            {
                char *str = client->receiveMsg();
                if (str != nullptr)
                {
                    if (str[0] == MSG_FLAG)
                    {
                        QMetaObject::invokeMethod(this, "writeConsole", Qt::QueuedConnection,
                                                  Q_ARG(QString, QString("Receive message from server: ") + (str + 1)));
                    }
                    else if (str[0] == FILE_FLAG)
                    {
                        int i = *(int *)(str + 1);
                        int len = *(int *)(str + 5);
                        uint t = *(int *)(str + 9);
                        packageManager->addData(i, len, qMin(PACKAGE_SIZE, len - i), t, str + 1 + 4 + 4 + 4);
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
}

void MainWindow::sendMsg()
{
    try
    {
        QString msg = msgEditLine->text().trimmed();
        if (msg.length() != 0)
        {
            client->SendMsg((MSG_FLAG + msg).toLatin1().data(), msg.length() + 1);
            console->write("Send message to server: " + msg + '\n');
        }
    }
    catch(const std::runtime_error &e)
    {
        console->write(QString(e.what()) + '\n');
    }
    msgEditLine->setText("");
}

void MainWindow::clearConsole()
{
    console->clear();
}

void MainWindow::selectFile()
{
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    if (codec == nullptr)
        codec = QTextCodec::codecForName("UTF-8");
    QString fileName = codec->toUnicode(QFileDialog::getOpenFileName().toLatin1());

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (fileName.length() != 0)
            console->write("Fail to open file:" + fileName + '\n');
        return;
    }

    qint64 fileSize = QFileInfo(fileName).size();
    if (fileSize > 200 * 1024)
    {
        console->write("Unable to send files larger than 200KB: " + fileName + '\n');
        return;
    }

    QDataStream in(&file);
    QByteArray s = file.readAll();
    uint t = std::time(0);
    int len = s.length();
    for (int i = 0; i < len; i += PACKAGE_SIZE)
    {
        QByteArray sub = s.sliced(i, qMin(PACKAGE_SIZE, len - i));
        QString msg = FILE_FLAG + QString::fromLatin1((char *)&i, 4) + QString::fromLatin1((char *)&len, 4)
                + QString::fromLatin1((char *)&t, 4) + QString::fromLatin1(sub.toStdString()) + QFileInfo(fileName).fileName() + QString::fromLatin1("\0");
        client->SendMsg(msg.toLatin1().data(), msg.length());
    }
    console->write("Send file to server: " + fileName + '\n');
    file.close();
}

void MainWindow::handleWriteConsole(const QString &msg)
{
    console->write(msg + '\n');
}

void MainWindow::handleReceiveFinish(const QString &fileName, const QByteArray &content)
{
    console->write("Receive file from server: " + fileName + '\n');
    QDir dataDir("files");
    if (!dataDir.exists())
        dataDir.mkpath(".");
    QFile dataFile(dataDir.filePath(fileName));
    if (dataFile.open(QIODevice::WriteOnly))
    {
        dataFile.write(content);
        dataFile.close();
    }
}
