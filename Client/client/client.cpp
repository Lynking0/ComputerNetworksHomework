#include "client.h"
#include <iostream>

using namespace std;

Client::Client(bool isIpv6_) : isIpv6(isIpv6_)
{
    // 初始化WinSock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        throw runtime_error("WSAStartup failed with error.");
}

void Client::createSocket()
{
    // 创建套接字
    if (isIpv6)
        clientSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    else
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        WSACleanup();
        throw runtime_error("socket creation failed with error.");
    }
}

void Client::setAddrAndPort(char *addr, u_short port)
{
    // 设置服务器地址和端口号
    if (isIpv6)
    {
        serverAddr6.sin6_family = AF_INET6;
        serverAddr6.sin6_port = htons(port);
        inet_pton(AF_INET6, addr, &serverAddr6.sin6_addr);
    }
    else
    {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(addr);
    }
}

void Client::connectServer()
{
    // 连接到服务器
    if (isIpv6)
        iResult = connect(clientSocket, (sockaddr*)&serverAddr6, sizeof(serverAddr6));
    else
        iResult = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        WSACleanup();
        throw runtime_error("connect failed with error.");
    }
}

void Client::SendMsg(char *msg, u_short len)
{
    // 发送消息给服务器
    iResult = send(clientSocket, msg, sizeof(char) * len, 0);
    if (iResult == SOCKET_ERROR)
        throw runtime_error("send failed with error.");
}

char *Client::receiveMsg()
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

Client::~Client()
{
    // 关闭套接字和WinSock
    closesocket(clientSocket);
    WSACleanup();
}
