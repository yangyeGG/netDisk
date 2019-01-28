#include "mysocket.h"
#include <QMessageBox>

USING_NAMESPACE_MYNET_SOCK
USING_NAMESPACE_STD

MySocket::MySocket()
{
}

MySocket::~MySocket()
{
}

void MySocket::clear()
{
    delete server;
    // 释放连接和进行结束工作
    closesocket(sClient);
    WSACleanup();
}

bool MySocket::InIt()
{
    WSADATA wsaData;
    server = new SOCKADDR_IN;
    //WSAStartup(0x0202, &wsaData); // Initialize Windows socket library
    if(WSAStartup(0x0202, &wsaData) != 0)
        return false;

    // 创建客户端套节字
    sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET指明使用TCP/IP协议族；
                                                         //SOCK_STREAM, IPPROTO_TCP具体指明使用TCP协议
    if(sClient == INVALID_SOCKET)
        return false;

    // 指明远程服务器的地址信息(端口号、IP地址等)
    memset(server, 0, sizeof(SOCKADDR_IN)); //先将保存地址的server置为全0
    server->sin_family = PF_INET;  //声明地址格式是TCP/IP地址格式
    server->sin_port = htons(PORT); //指明连接服务器的端口号
    server->sin_addr.s_addr = inet_addr(SERVER_ADDRESS); //指明连接服务器的IP地址
    return true;
}

bool MySocket::ConnecttoHost()
{
    if(connect(sClient,(struct sockaddr*)server,sizeof(SOCKADDR_IN)))
        return false;
    else
        return true;
}


int MySocket::sendData(char* data,int size)
{
    ret = send(sClient,data,size,0);
    return ret;
}

int MySocket::sendData(Xml_Info* data,int size)
{
    ret = send(sClient,(char*)data,size,0);
    return ret;
}

int MySocket::recvData(char* data,int size)
{
    ret = recv(sClient,data,size*sizeof(char),0);
    return ret;
}

int MySocket::recvData(Xml_Info* data,int size)
{
    ret = recv(sClient,(char*)data,size,0);
    return ret;
}

