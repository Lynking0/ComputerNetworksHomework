#ifndef CLIENT_H
#define CLIENT_H

#include "WS2tcpip.h"
#include "winsock2.h"

#pragma comment (lib, "ws2_32.lib") // 不加会报 LNK2019 无法解析的外部符号

class Client
{
private:
    bool isIpv6;
    WSADATA wsaData;
    int iResult;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    sockaddr_in6 serverAddr6;
public:
    Client(bool);
    ~Client();
    void createSocket();
    void setAddrAndPort(char *addr, u_short port);
    void connectServer();
    void SendMsg(char *msg, u_short len);
    char *receiveMsg();
};

#endif // CLIENT_H
