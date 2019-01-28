#include "mysystemtray.h"

MySystemTray::MySystemTray(QWidget *parent) : QWidget(parent)
{
    //模态窗口
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    this->parent = parent;
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon(":/image/logo.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::trUtf8("记录生活 更记录你~"));
    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    //在系统托盘显示此对象

    //建立托盘操作的菜单
    createActions();
    createMenu();

    mSysTrayIcon->show();

}

MySystemTray::~MySystemTray()
{
}

void MySystemTray::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
        //显示消息球，1s后自动消失
        //第一个参数是标题
        //第二个参数是消息内容
        //第三个参数图标
        //第四个参数是超时毫秒数
        mSysTrayIcon->showMessage(QObject::trUtf8("Message Title"),
                                          QObject::trUtf8("记录生活 更记录你~"),
                                          QSystemTrayIcon::Information,
                                          1000);

        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        //双击后显示主程序窗口
        parent->show();
        break;
    default:
        break;
    }
}

void MySystemTray::createActions()
{
    mShowMainAction = new QAction(QObject::trUtf8("显示主界面"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));

    mExitAppAction = new QAction(QObject::trUtf8("退出"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));

}

void MySystemTray::createMenu()
{
    mMenu = new QMenu(this);
    //新增菜单项---显示主界面
    mMenu->addAction(mShowMainAction);
    //增加分隔符
    mMenu->addSeparator();
    //新增菜单项---退出程序
    mMenu->addAction(mExitAppAction);
    //把QMenu赋给QSystemTrayIcon对象
    mSysTrayIcon->setContextMenu(mMenu);
}

// 当在系统托盘点击菜单内的显示主界面操作

void MySystemTray::on_showMainAction()
{
    parent->show();
}


// 当在系统托盘点击菜单内的退出程序操作

void MySystemTray::on_exitAppAction()
{
    this->close();
    parent->close();
    exit(0);
}

