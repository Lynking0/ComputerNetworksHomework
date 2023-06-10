#ifndef CLIENT_H
#define CLIENT_H

#include "winsock.h"

#pragma comment (lib, "ws2_32.lib") // 不加会报 LNK2019 无法解析的外部符号

class Client
{
private:
    WSADATA wsaData;
    int iResult;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
public:
    Client();
    ~Client();
    void createSocket();
    void setAddrAndPort();
    void connectServer();
    void SendMsg();
};

#endif // CLIENT_H
