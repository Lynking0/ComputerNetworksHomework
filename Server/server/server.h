#ifndef SERVER_H
#define SERVER_H

#include "WS2tcpip.h"
#include "winsock2.h"

#pragma comment (lib, "ws2_32.lib") // 不加会报 LNK2019 无法解析的外部符号

class Server
{
private:
    bool isIpv6;
    WSADATA wsaData;
    int iResult;
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    sockaddr_in6 serverAddr6;
    SOCKET clientSocket;
public:
    Server(bool);
    ~Server();
    void createSocket();
    void enableIpv6();
    void setAddrAndPort(char *addr, u_short port);
    void bindSocket();
    void startListen();
    void waitClient();
    void SendMsg(char *msg, u_short len);
    char *receiveMsg();
};

#endif // SERVER_H
