#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <QObject>
#include <QMap>

using namespace std;

class PackageManager : public QObject
{
    Q_OBJECT

public:
    PackageManager(QObject *parent = nullptr);
    void addData(int startIdx, int totLen, int len, int id, char *data);

private:
    QMap<uint, char *> datas;

signals:
    void receiveFinish(const QString &fileName, const QByteArray &content);
};

#endif // PACKAGEMANAGER_H
