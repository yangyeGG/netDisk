#ifndef RECVTHREAD_H
#define RECVTHREAD_H
#include <QThread>
#include "mysocket.h"
#include "NetDisk_Type.h"

USING_NAMESPACE_MYNET_SOCK
NAMESPACE_MYNET_RECVTHREAD

class RecvThread : public QThread
{
public:
    RecvThread();
    ~RecvThread();
    virtual void run();
    bool recv_Xml();

signals:
    void SIG_LinkErr() //连接失败
    {}
    void recv_xml_info_error()//文件信息接受失败
    {}
    void SIG_bind_error(){} //绑定失败
private:
    MySocket* Recv_Socket;

};

NAMESAPCE_END
#endif // RECVTHREAD_H
