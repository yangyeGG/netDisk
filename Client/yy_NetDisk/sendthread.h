#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include <QThread>
#include <QMessageBox>
#include "mysocket.h"
#include "NetDisk_Type.h"

USING_NAMESPACE_MYNET_SOCK
NAMESPACE_MYNET_SENDTHREAD

class SendThread : public QThread
{
public:
    SendThread();
    ~SendThread();
    virtual void run();
    bool send_Xml(std::string);
    std::string FileName;

signals:
    void SIG_LinkErr(){} //¡¨Ω” ß∞‹
    void SIG_bind_error(){} //∞Û∂® ß∞‹
    //void SIG_send();
private:
    MySocket* Send_Socket;

};

NAMESAPCE_END
#endif // SENDTHREAD_H
