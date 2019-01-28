#include "sendthread.h"
#include <QFile>
USING_NAMESPACE_MYNET_SENDTHREAD
SendThread::SendThread()
{
    Send_Socket = new MySocket;
}

SendThread::~SendThread()
{
    delete Send_Socket;
}

void SendThread::run()
{
    if(!send_Xml(FileName))
        QMessageBox::warning(nullptr,"Error","与服务器断开连接,请检查网络!");
}

bool SendThread::send_Xml(std::string filename)
{
    delete Send_Socket;
    Send_Socket = new MySocket;
    if(!Send_Socket->InIt())//bind 绑定地址
    {
        emit SIG_bind_error();
        return false;
    }

    if(!Send_Socket->ConnecttoHost())//与服务器建立连接
    {
        emit SIG_LinkErr();//发送连接失败信号
        return false;
    }
    else
    {
        Xml_Info info;
        int ret;
        char buff[MSGSIZE];
        FILE* fp = fopen(filename.c_str(),"rb");
        if(fp == nullptr)
            return false;
        fseek(fp,0,SEEK_END);
        info.size = ftell(fp);
        strcpy(info.name,filename.c_str());
        rewind(fp);
        ret = Send_Socket->sendData(&info,sizeof(info));
        if(ret == sizeof(info))
        {
            while(!feof(fp))
            {
                ret = fread(buff,1,MSGSIZE,fp);
                Send_Socket->sendData(buff,ret);
            }
        }
        fclose(fp);
    }
    Send_Socket->clear();
    return true;
}
