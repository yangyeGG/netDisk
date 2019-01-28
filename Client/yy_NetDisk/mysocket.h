#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <winsock2.h>
#include <stdio.h>
#include "NetDisk_Type.h"
//#pragma comment(lib, "ws2_32.lib") //vs下这样使用
NAMESPACE_MYNET_SOCK

#define SERVER_ADDRESS "120.79.208.50" //服务器端IP地址
#define PORT           11112         //服务器的端口号
#define MSGSIZE        1024         //收发缓冲区的大小


class MySocket
{
public:
    MySocket();
    ~MySocket();

public:
    bool ConnecttoHost();
    int sendData(char* data,int size);
    int sendData(Xml_Info* data,int size);
    int recvData(char* data,int size);//注意size要比data的大小小1，留一个位置存放\0
    int recvData(Xml_Info* data,int size);
    bool InIt(); //加载
    void clear(); //释放


public:
    //连接所用套节字
    SOCKET sClient;
    //保存远程服务器的地址信息
    SOCKADDR_IN* server;
    //收发缓冲区
    char szMessage[MSGSIZE];
    //成功接收字节的个数
    int ret;

};
NAMESAPCE_END
#endif // MYSOCKET_H
