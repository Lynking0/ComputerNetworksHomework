#include "server.h"
#include <iostream>

using namespace std;

Server::Server(bool isIpv6_) : isIpv6(isIpv6_)
{
    // 初始化WinSock
    iResult = WSAStartup(MAKEWORD(1, 1), &wsaData);
    if (iResult != NO_ERROR)
        throw runtime_error("WSAStartup failed with error.");
}

void Server::createSocket()
{
    // 创建套接字
    if (isIpv6)
        serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    else
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        WSACleanup();
        throw runtime_error("socket creation failed with error.");
    }
}

void Server::enableIpv6()
{
    int optVal = 1;
    iResult = setsockopt(serverSocket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&optVal, sizeof(optVal));
    if (iResult == SOCKET_ERROR)
    {
        closesocket(serverSocket);
        WSACleanup();
        throw runtime_error("socket option set failed with error.");
    }
}

void Server::setAddrAndPort(char *addr, u_short port)
{
    // 设置服务器地址和端口号
    if (isIpv6)
    {
        serverAddr6.sin6_family = AF_INET6;
        serverAddr6.sin6_port = htons(port);
        serverAddr6.sin6_addr = in6addr_any;
    }
    else
    {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(addr);
    }
}

void Server::bindSocket()
{
    // 将套接字绑定到对应IP地址和端口号上
    if (isIpv6)
        iResult = bind(serverSocket, (sockaddr*)&serverAddr6, sizeof(serverAddr6));
    else
        iResult = bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
    {
        std::cout << WSAGetLastError() << std::endl;
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

void Server::SendMsg(char *msg, u_short len)
{
    // 发送消息给服务器
    iResult = send(clientSocket, msg, sizeof(char) * len, 0);
    if (iResult == SOCKET_ERROR)
        throw runtime_error("send failed with error.");
}

char *Server::receiveMsg()
{
    // 接收客户端发送的消息
    char *recvBuf = new char[32 * 1024];
    iResult = recv(clientSocket, recvBuf, sizeof(char) * 32 * 1024, 0);
    if (iResult > 0)
    {
        recvBuf[iResult] = '\0';
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
