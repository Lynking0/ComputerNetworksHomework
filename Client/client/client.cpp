#include "client.h"
#include <iostream>

using namespace std;

Client::Client()
{
    // 初始化WinSock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        throw runtime_error("WSAStartup failed with error.");
}

void Client::createSocket()
{
    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        WSACleanup();
        throw runtime_error("socket creation failed with error.");
    }
}

void Client::setAddrAndPort()
{
    // 设置服务器地址和端口号
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(52354); // 对应服务器的端口号
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 对应服务器的IP地址
}

void Client::connectServer()
{
    // 连接到服务器
    iResult = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        WSACleanup();
        throw runtime_error("connect failed with error.");
    }
}

void Client::SendMsg()
{
    // 发送消息给服务器
    char sendBuf[] = "Hello, server!";
    iResult = send(clientSocket, sendBuf, sizeof(sendBuf), 0);
    if (iResult == SOCKET_ERROR)
        throw runtime_error("send failed with error.");
}

Client::~Client()
{
    // 关闭套接字和WinSock
    closesocket(clientSocket);
    WSACleanup();
}
