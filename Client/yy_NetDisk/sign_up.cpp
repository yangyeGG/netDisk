#include "sign_up.h"
#include "ui_sign_up.h"
#include "NetDisk_Type.h"
#include "md5.h"
#include <QMessageBox>
#include <QPainter>


authcode auth_code1;// authcode
usr_info info1;
int btn_status1 = 0; //识别按钮 0 logn_up  1 auth_code

Sign_up::Sign_up(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sign_up)
{
    ui->setupUi(this);
    this->setWindowTitle("sign up!");//title

    ui->Usename->setText("Tel");
    ui->Usename->setStyleSheet("color:gray;");

    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);//窗口属性
 //   this->setStyleSheet("background-color: white"); //background color


    //tcp
     socket = new QTcpSocket;
     timer = new QTimer(this);
}

Sign_up::~Sign_up()
{
    delete ui;
    delete socket;
    delete timer;
}

void Sign_up::socket_connect() //链接成功
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(recv_data()));
    if(!btn_status1)
        socket->write((const char*)&info1,sizeof(usr_info));
    else
    {
        socket->write((const char*)&auth_code1,sizeof(auth_code1));
        socket->flush();
        socket->write((const char*)&auth_code1,sizeof(auth_code1));
    }
}

void Sign_up::socket_error() //链接失败
{
    QMessageBox::warning(this,"error","请检查网络是否正常!");
    this->close();
}


void Sign_up::recv_data()//recv data
{
 QString data = socket->readAll();

 if(data == "sign_up success!")
 {
     QMessageBox::about(this,"温馨提示!","注册成功!");
     this->close();
 }
 if(data == "Usr exists!")
 {
    QMessageBox::warning(this,"抱歉!","该用户已存在!");
    ui->Usename->clear();
    ui->Usename->setFocus();
    return;
 }

 if(data == "sign_up Auth_code error!")
 {
    QMessageBox::warning(this,"抱歉!","请输入正确验证码!");
    ui->Authcode->clear();
    ui->Authcode->setFocus();
    return;
 }
}



void Sign_up::on_auth_code_clicked()//验证码
{
    delete socket;
    socket =new QTcpSocket;
    btn_status1 = 1;

    //正则表达式校验
    QRegExp regexp("^((13[0-9])|(14[5,7])|(15[0-3,5-9])|(17[0,3,5-8])|(18[0-9])|166|198|199|(147))\\d{8}$");
    //ui->Usr->setValidator(new QRegExpValidator(regexp,this));
    bool match = regexp.exactMatch(ui->Usename->text());
    if(!match)
    {
        QMessageBox::warning(this,"警告!","请输入正确手机号!");
        ui->Usename->clear();
        ui->Usename->setFocus();
        return;
    }

    QString usr = ui->Usename->text();
    strcpy(auth_code1._Usr,usr.toUtf8().data());

    connect(socket,SIGNAL(connected()),this,SLOT(socket_connect()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socket_error()));
    socket->connectToHost("120.79.208.50",11111);

    ui->auth_code->setEnabled(false);


    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1000);

}

void Sign_up::update()
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
        cnt = 60;
        timer->stop();

    }

}

void Sign_up::on_sign_up_clicked()//注册
{
    delete socket;
    socket =new QTcpSocket;
    btn_status1 = 0;
    info1.e_mode = LOGN_UP;

    //正则表达式校验
    QRegExp regexp("^((13[0-9])|(14[5,7])|(15[0-3,5-9])|(17[0,3,5-8])|(18[0-9])|166|198|199|(147))\\d{8}$");
    if(!(regexp.exactMatch(ui->Usename->text())))
    {
        QMessageBox::warning(this,"警告!","请输入正确手机号!");
        ui->Usename->clear();
        ui->Usename->setFocus();
        return;
    }

    if(ui->Passwd->text().isEmpty())
    {
        QMessageBox::warning(this,"警告!","密码不能为空！");
        ui->Passwd->clear();
        ui->Passwd_2->clear();
        ui->Passwd->setFocus();
        return;
    }

    if(ui->Passwd->text() != ui->Passwd_2->text())
    {
        QMessageBox::warning(this,"警告!","两次密码不相同！");
        ui->Passwd_2->clear();
        ui->Passwd_2->setFocus();
        return;
    }

    if(ui->Authcode->text().isEmpty())
    {
       QMessageBox::warning(this,"警告!","验证码不能为空！");
       ui->Authcode->clear();
       ui->Authcode->setFocus();
       return;
    }

    strcpy(info1._usr,ui->Usename->text().toUtf8().data());
    strcpy(info1.Passwd,md5_encrypt(ui->Passwd_2->text().toUtf8().data(),ui->Passwd_2->text().size()));
    strcpy(info1.AuthCode,ui->Authcode->text().toUtf8().data());

    connect(socket,SIGNAL(connected()),this,SLOT(socket_connect()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socket_error()));
    socket->connectToHost("120.79.208.50",11111);
}

void Sign_up::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //画渐变背景--线性渐变
    QLinearGradient objLinear(rect().topLeft(),rect().bottomRight());
    objLinear.setColorAt(0,Qt::white);
    objLinear.setColorAt(1,Qt::black);
    painter.fillRect(rect(),objLinear);
}
