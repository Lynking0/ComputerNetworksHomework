#include "server.h"
#include <iostream>

using namespace std;

Server::Server()
{
    // 初始化WinSock
    iResult = WSAStartup(MAKEWORD(1, 1), &wsaData);
    if (iResult != NO_ERROR)
        throw runtime_error("WSAStartup failed with error.");
}

void Server::createSocket()
{
    // 创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        WSACleanup();
        throw runtime_error("socket creation failed with error.");
    }
}

void Server::setAddrAndPort(char *addr, u_short port)
{
    // 设置服务器地址和端口号
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(addr);
}

void Server::bindSocket()
{
    // 将套接字绑定到对应IP地址和端口号上
    iResult = bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
    {
        closesocket(serverSocket);
        WSACleanup();
        throw runtime_error("socket bind failed with error.");
    }
}

void Server::startListen()
{
    // 开始监听连接请求
    iResult = listen(serverSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(serverSocket);
        WSACleanup();
        throw runtime_error("listen failed with error.");
    }
}

void Server::waitClient()
{
    // 等待客户端连接请求
    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        closesocket(serverSocket);
        WSACleanup();
        throw runtime_error("accept failed with error.");
    }
}

char *Server::receiveMsg(bool &isEnd)
{
    // 接收客户端发送的消息
    char *recvBuf = new char[1024];
    iResult = recv(clientSocket, recvBuf, sizeof(recvBuf), 0);
    if (iResult > 0)
    {
        if (recvBuf[iResult - 1] == '\0')
            isEnd = true;
        else
        {
            isEnd = false;
            recvBuf[iResult] = '\0';
        }
        return recvBuf;
    }
    else if (iResult == 0)
    {
        delete[] recvBuf;
        throw runtime_error("connection closed by client");
    }
    else
    {
        delete[] recvBuf;
        return nullptr;
    }
}

Server::~Server()
{
    // 关闭套接字和WinSock
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
