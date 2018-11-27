#include <QLabel>
#include <QFile>
#include <QString>
#include <cstdio>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QTcpSocket>
#include <QUrl>
#include <string.h>
#include <QRegExp>
#include <QTimer>
#include "xmb.h"
#include "md5.h"
#include "login.h"
#include "logon.h"
#include "ui_login.h"
//#include "my_qlabel.h"

#pragma pack(1)

typedef struct _usr_info
{
    bool mode;
    char _usr[20];
    char Passwd[33];

}usr_info;
typedef struct authcode_
{
    char _Usr[20];

}authcode;

#pragma pack()

bool enterBtn(QPoint pp, QLabel *lab);

int login_stauts = 0;//login_status  0 login  1 mail_login
int btn_status = 0; //识别按钮 0 login  1 auth_code
usr_info info;//
authcode auth_code;// authcode

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    setMouseTracking(true);
    ui->Usr->setStyleSheet("color:gray");
    ui->label->setMouseTracking(true);
    ui->mail_login->setMouseTracking(true);
    ui->losepasswd->setMouseTracking(true);

    ui->Usr->setText("Tel");
    ui->auth_code->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->authcode->hide();
    ui->auth_code->hide();
    this->setWindowTitle("欢迎使用yy网盘!");//title
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);//窗口属性
    this->setStyleSheet("background-color: white"); //background color
    ui->Login->setText(" ");
    ui->Login->setStyleSheet("border:0px groove gray;border-radius:5px;padding:2px 4px;background-image: url(:/images/img/login.png); background-size: 100% 100%;");//login style


    //读取本地日志文件
    FILE* fp = fopen("./NetDisk.init","rb");
    if(fp != NULL)
    {
        fseek(fp,0,SEEK_END);
        int size = ftell(fp);
        rewind(fp);
        char buff[100];
        char usr[100] = "";
        char passwd[100] = "";
        int ischecked_getpasswd;
        fread(buff,1,size,fp);
        sscanf(buff,"%s%s%d",usr,passwd,&ischecked_getpasswd);
        if(ischecked_getpasswd)
        {
            ui->Usr->setText(usr);
            ui->passwd->setText(passwd);
            ui->checkBox->setChecked(true);
        }
        fclose(fp);
    }




    /*QFile file_init("NetDisk.init");
    if(file_init.open(f,QIODevice::ReadOnly | QIODevice::Text))
    {
        if(ui->checkBox->isChecked())
        {
            QString data = file_init.readAll();

            char usr[100] = "";
            char passwd[100] = "";
            sscanf(data.toUtf8().data(),"%s%s",usr,passwd);
            ui->Usr->setText(usr);
            ui->passwd->setText(passwd);
        }
        file_init.close();
        f = NULL;
    }*/


    //TCP
    socket = new QTcpSocket;
    timer = new QTimer(this);
}

login::~login()
{
    delete ui;
    delete socket;
    delete timer;
}




//登录
void login::on_Login_clicked()
{
    //TCP
    delete socket;
    socket = new QTcpSocket;
    socket->flush();

    //正则表达式校验
    QRegExp regexp("^((13[0-9])|(14[5,7])|(15[0-3,5-9])|(17[0,3,5-8])|(18[0-9])|166|198|199|(147))\\d{8}$");
    ui->Usr->setValidator(new QRegExpValidator(regexp,this));
    bool match = regexp.exactMatch(ui->Usr->text());
    if(!match)
    {
        QMessageBox::warning(this,"警告!","请输入正确手机号!");
        ui->Usr->clear();
        ui->passwd->clear();
        ui->authcode->clear();
        ui->Usr->setFocus();
        return;
    }

    btn_status = 0;
    if(login_stauts) // 短信登录
    {
        //QMessageBox::warning(this,"hh","mail_login");
        info.mode = false; // tel_login
        QString usr = ui->Usr->text();

        strcpy(info._usr,usr.toUtf8().data());
        strcpy(info.Passwd,ui->authcode->text().toUtf8().data());


        connect(socket,SIGNAL(connected()),this,SLOT(socket_connect()));
        connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socket_error()));

    }
    else // 登录
    {
        info.mode = true;//ord_login
        QString usr = ui->Usr->text();
        QString passwd = ui->passwd->text();
        strcpy(info._usr,usr.toUtf8().data());
        //md5
        strcpy(info.Passwd,md5_encrypt(passwd.toUtf8().data(),passwd.size()));
        qDebug()<<info._usr;
        qDebug()<<info.Passwd;
        //info._usr[32] = '\0';
        info.Passwd[32] = '\0';


        connect(socket,SIGNAL(connected()),this,SLOT(socket_connect()));
        connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socket_error()));

       // QMessageBox::about(this,"温馨提示!","连接服务器中。。。");

    }
    socket->connectToHost("120.79.208.50",11111);

}

//link success  send data
void login::socket_connect()
{
    //QMessageBox::about(this,"success","link success!");
    connect(socket,SIGNAL(readyRead()),this,SLOT(recv_data()));
    if(!btn_status)
        socket->write((const char*)&info,sizeof(usr_info));
    else
    {
        socket->write((const char*)&auth_code,sizeof(auth_code));
        socket->flush();
        socket->write((const char*)&auth_code,sizeof(auth_code));
    }

}

//recv data
void login::recv_data()
{
 QString data = socket->readAll();
 //ui->Usr->setText(data);

 if(data == "success!")
 {
     int ischecked_getpasswd;//记住密码是否勾选
     if(ui->checkBox->isChecked())
         ischecked_getpasswd = 1;
     else
         ischecked_getpasswd = 0;

     //写入本地日志文件
    FILE* fp = fopen("./NetDisk.init","wb");
    //QString data = ui->Usr->text() + " " + ui->passwd->text() + " ";
    char data[100];
    sprintf(data,"%s %s %d ",ui->Usr->text().toUtf8().data(),ui->passwd->text().toUtf8().data(),ischecked_getpasswd);
    fwrite(data,1,strlen(data),fp);
    fclose(fp);

    /*
    QFile file_init("NetDisk.init");
    file_init.open(f,QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    if(ui->checkBox->isChecked())
    {
        QString data = ui->Usr->text() + " " + ui->passwd->text();
        file_init.write(data.toUtf8().data());
    }
    file_init.close();
    f = NULL;*/


    XMB x;
    this->close();
    x.exec();
 }
 if(data == "Usr or passwd error!")
 {
     QMessageBox::warning(this,"error","用户名或密码错误!");
     ui->passwd->clear();
     ui->passwd->setFocus();
     return;
 }
 if(data == "No Usr!")
 {
     QMessageBox::warning(this,"error","用户名不存在!");
     ui->Usr->clear();
     ui->passwd->clear();
     ui->Usr->setFocus();
     return;
 }

 if(data == "Auth_code error!")
 {
     QMessageBox::warning(this,"error","验证码错误!");
     ui->authcode->clear();
     ui->authcode->setFocus();
     return;
 }
}

//link fail
void login::socket_error()
{
    QMessageBox::warning(this,"error","请检查网络是否正常!");
    this->close();
}


//auth_code clicked  正则表达式校验
void login::on_auth_code_clicked()
{
     delete socket;
     socket =new QTcpSocket;
     btn_status = 1;
     //正则表达式校验
     QRegExp regexp("^((13[0-9])|(14[5,7])|(15[0-3,5-9])|(17[0,3,5-8])|(18[0-9])|166|198|199|(147))\\d{8}$");
     //ui->Usr->setValidator(new QRegExpValidator(regexp,this));
     bool match = regexp.exactMatch(ui->Usr->text());
     if(!match)
     {
         QMessageBox::warning(this,"警告!","请输入正确手机号!");
         ui->Usr->clear();
         ui->Usr->setFocus();
         return;
     }


     QString usr = ui->Usr->text();
     strcpy(auth_code._Usr,usr.toUtf8().data());

     connect(socket,SIGNAL(connected()),this,SLOT(socket_connect()));
     connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socket_error()));
     socket->connectToHost("120.79.208.50",11111);

     ui->auth_code->setEnabled(false);


     connect(timer,SIGNAL(timeout()),this,SLOT(update()));
     timer->start(1000);
}

void login::update()
{
    static int cnt = 60;
    char data[100];
    sprintf(data,"已发送(%d)",cnt);
    ui->auth_code->setText(data);
    --cnt;
    if(cnt == 0)
    {
        ui->auth_code->setText("获取验证码");
        ui->auth_code->setEnabled(true);
        timer->stop();
    }

}




//mousePressEvent
void login::mousePressEvent(QMouseEvent* event)
{


    if(event->button() == Qt::LeftButton)
    {
        //int x=event->globalX();
        //int y=event->globalY();
        //qDebug()<<x<<" "<<y;
        //login_label
        if(enterBtn(event->pos(),ui->label))
        {
            ui->label->setStyleSheet("color:red;border:1px solid gray;border-radius:3px;");
            ui->Usr->setText("Tel");
            ui->Usr->setStyleSheet("color:gray");
            //ui->passwd->setText("Password");
            ui->passwd->show();
            ui->authcode->hide();
            ui->auth_code->hide();
            ui->checkBox->show();
            ui->checkBox_2->show();
            ui->losepasswd->show();
            ui->passwd->clear();
            login_stauts = 0;
        }
        if(enterBtn(event->pos(),ui->logon)) //zhuce
        {
            ui->logon->setStyleSheet("color:red;border:1px solid gray;border-radius:3px;");
            QDesktopServices::openUrl(QUrl("www.baidu.com"));
        }

        if(enterBtn(event->pos(),ui->mail_login))//mail_login
        {
            ui->Usr->setText("Tel");
            ui->Usr->setStyleSheet("color:gray");
            ui->mail_login->setStyleSheet("color:red;border:1px solid gray;border-radius:3px;");
            ui->passwd->hide();
            ui->authcode->show();
            ui->auth_code->show();
            ui->checkBox->hide();
            ui->checkBox_2->hide();
            ui->losepasswd->hide();
            login_stauts = 1;
        }
        if(enterBtn(event->pos(),ui->losepasswd)) // losepasswd
        {
            ui->losepasswd->setStyleSheet("color:red;border:1px solid gray;border-radius:3px;");
            QDesktopServices::openUrl(QUrl("www.baidu.com"));
        }
    }


}

//mouseMoveEvent;
void login::mouseMoveEvent(QMouseEvent * event)
{
//    int x=event->globalX();
//    int y=event->globalY();
//    qDebug()<<"x="<<x<<" y="<<y;
    event->accept();

    if(enterBtn(event->pos(),ui->label))
    {
        ui->label->setStyleSheet("color:blue;");
    }
    else if(enterBtn(event->pos(),ui->mail_login))
    {
        ui->mail_login->setStyleSheet("color:red;");
    }
    else if(enterBtn(event->pos(),ui->losepasswd))
    {
        ui->losepasswd->setStyleSheet("color:red;");
    }
    else if(enterBtn(event->pos(),ui->logon))
    {
        ui->logon->setStyleSheet("color:red;");;
    }
    else
    {
        if(login_stauts == 0) ui->label->setStyleSheet("color:red;");
        else ui->label->setStyleSheet("color:black;");

        if(login_stauts == 1)  ui->mail_login->setStyleSheet("color:red;");
        else ui->mail_login->setStyleSheet("color:black;");
        ui->losepasswd->setStyleSheet("color:blue;");
        ui->logon->setStyleSheet("color:blue;");
    }

}


bool enterBtn(QPoint pp, QLabel *lab)
{
   int height = lab->height();
   int width = lab->width();
   QPoint btnMinPos = lab->pos();
   QPoint btnMaxPos = lab->pos();
   btnMaxPos.setX(lab->pos().x()+width);
   btnMaxPos.setY(lab->pos().y()+height);
   if(pp.x() >= btnMinPos.x() && pp.y() >= btnMinPos.y()
       && pp.x() <= btnMaxPos.x() && pp.y() <= btnMaxPos.y())
       return true;
   else
       return false;
}



