#include "frmmain.h"
#include "ui_frmmain.h"
#include "mysystemtray.h"
#include "waitdig.h"
#include "xmb.h"
#include <QtGui>
#include <QMessageBox>

frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    this->InitStyle();
    this->InitForm();

    //模态窗口
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    //
    MySystemTray* w = new MySystemTray(this);
    w->hide();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmMain::InitStyle()
{
    this->mousePressed = false;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
}

void frmMain::InitForm()
{
    bg = "main4.png";
    QList<QToolButton *> btns = this->findChildren<QToolButton *>();
    foreach (QToolButton * btn, btns)
    {
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }
}

void frmMain::buttonClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString objName = btn->objectName();
    if (objName == "btnCOMTool") // 更换背景
    {
        if (bg == "main1.png")
        {
            bg = "main2.png";
        }
        else if (bg == "main2.png") {
            bg = "main3.png";
        }
        else if (bg == "main3.png") {
            bg = "main4.png";
        }
        else if (bg == "main4.png") {
            bg = "main5.png";
        }
        else if (bg == "main5.png") {
            bg = "main1.png";
        }
        QString qss = QString("QWidget#frm{background-image: url(:/image/%1);}").arg(bg);
        qss += "QToolButton{color:#E7ECF0;background-color:rgba(0,0,0,0);border-style:none;}";
        this->setStyleSheet(qss);
    }
    else if (objName == "btnTCPTool")//退出系统
    {
        this->close();
    }
    else if(objName == "btnMyNet") // 网盘
    {
        //QMessageBox::about(this,"抱歉!","该功能暂未实现,敬请期待!");
        this->hide();
        XMB w;
        w.show();
        w.exec();
    }
    else if (objName == "btnEnjoy") //好友系统
    {
        QMessageBox::about(this,"抱歉!","该功能暂未实现,敬请期待!");
    }
    else if (objName == "btnFind") //找资源
    {
        QDesktopServices::openUrl(QUrl("www.ityy520.com"));
    }
    else if (objName == "btnMore") // 更多
    {
        QMessageBox::about(this,"抱歉!","更多功能,敬请期待!");
        WaitDig a;
        a.show();
        a.exec();
    }

}
