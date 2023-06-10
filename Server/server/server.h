#ifndef SERVER_H
#define SERVER_H

#include "winsock.h"

#pragma comment (lib, "ws2_32.lib") // 不加会报 LNK2019 无法解析的外部符号

class Server
{
private:
    WSADATA wsaData;
    int iResult;
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    SOCKET clientSocket;
public:
    Server();
    ~Server();
    void createSocket();
    void setAddrAndPort();
    void bindSocket();
    void startListen();
    void waitClient();
    void receiveMsg();
};

#endif // SERVER_H
