#include "package_manager.h"
#include <cstring>

PackageManager::PackageManager(QObject *parent)
    : QObject{parent}
{

}

void PackageManager::addData(int startIdx, int totLen, int len, int id, char *data)
{
    char *str = datas.value(id, nullptr);
    if (str == nullptr)
    {
        str = new char[4 + 64 + totLen];
        datas.insert(id, str);
        char *fileName = data + len;
        strcpy(str + 4, fileName);
        int *tot = (int *)str;
        *tot = 0;
    }
    int *tot = (int *)str;
    *tot += len;
    for (int i = 0; i < len; ++i)
        str[4 + 64 + startIdx + i] = data[i];
    if (*tot == totLen)
    {
        QString fileName(str + 4);
        QByteArray content = QByteArray(str + 4 + 64, totLen);
        emit receiveFinish(fileName, content);
        delete[] str;
    }
}
