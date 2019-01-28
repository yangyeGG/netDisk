#include "xmb.h"
#include "ui_xmb.h"
#include "myxml.h"
#include "md5.h"
#include "mysystemtray.h"
#include "sendthread.h"
#include "recvthread.h"
#include "myxml.h"

USING_NAMESPACE_MYNET_SENDTHREAD
USING_NAMESPACE_MYNET_RECVTHREAD
USING_NAMESPACE_MYNET_XML

#define DELETEFILE(x,y) if(x){\
    for(i = 0;i < y;++i){\
    if(ButtonArry[i] != nullptr)\
        delete ButtonArry[i];}}

#define VALUE_FALSE(x) x = false;

static bool stat = false;
XMB::XMB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XMB)
{
    ui->setupUi(this);
    setWindowTitle("MyNet");//title
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);//窗口属性

    for(int i = 0;i<MAXNUM;++i)
        ButtonArry[i] = nullptr;

    mMenu();//菜单栏
    CreateWeb();//找资源窗口
}

XMB::~XMB()
{
    delete ui;
    delete menu;
    delete MyMenuBar;
}
//获取用户名
bool XMB::getUsr(std::string& Usr)
{
    QString USR("");
    FILE* fp = fopen("./log/NetDisk.ini","rb");
    if(fp != nullptr)
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
        fclose(fp);
        Usr += usr;
        return true;
    }
   return false;
}

void XMB::Init()//chushihua
{
    std::string name;
    if(!getUsr(name))
        return;
    strcpy(com_tmp.Usr_name,name.c_str());
    com_tmp.com = INIT_FILE;
    strcpy(com_tmp.Path,"./dsad");

    MyXml xml;
    xml.m_ComFile.push_back(com_tmp);
    xml.WriteXml(this,"./log/xml/init.xml");

    MySocket sock_recv;

    if(!sock_recv.InIt())
    {
        QMessageBox::warning(this,"error","bind error");
        return;
    }
    if(!sock_recv.ConnecttoHost())
    {
        QMessageBox::warning(this,"error","link error");
        return;
    }

    Xml_Info info;
    int ret;
    char buff[MSGSIZE];
    FILE* fp = fopen("./log/xml/init.xml","rb");
    if(fp == nullptr)
        return ;
    fseek(fp,0,SEEK_END);
    info.size = ftell(fp);
    strcpy(info.name,"init.xml");
    rewind(fp);
    ret = sock_recv.sendData(&info,sizeof(info));
    if(ret == sizeof(info))
    {
        while(!feof(fp))
        {
            ret = fread(buff,1,MSGSIZE,fp);
            sock_recv.sendData(buff,ret);
        }
    }
    fclose(fp);

    Xml_Info m_xml;

    fp = fopen("./log/xml/recv.xml","wb");
    if(nullptr == fp)
        return;

    ret = sock_recv.recvData(&m_xml,sizeof(m_xml)*sizeof(char));

    if(ret == -1)
    {
        QMessageBox::about(this,"error",strerror(errno));
        return;
    }

    int t=0;
    if(sizeof(m_xml) == ret)
    {
        while(t != m_xml.size)
        {
            int tmp;
            tmp = recv(sock_recv.sClient,buff,1024,0);
            t+=tmp;
            fwrite(buff,1,tmp,fp);
        }
    }
    fclose(fp);
    sock_recv.clear();

    //cat xml
    if(xml.readFile("./log/xml/recv.xml"))
        File_All.assign(xml.m_DataSwap.begin(),xml.m_DataSwap.end());
}

void XMB::mMenu()
{
    MyMenuBar = new QMenuBar;
    QAction* last = new QAction("Last");
    last->setIcon(QIcon(":/image/FileIcon/TxtFile.png"));
    MyMenuBar->addAction(last);
    MyMenuBar->addMenu(CreateMenu("File"));
    ui->menu_layout->addWidget(MyMenuBar);
    MyMenuBar->addMenu(CreateMenu("About"));
    connect(last,SIGNAL(triggered(bool)),this,SLOT(OnLast()));
}

QMenu* XMB::CreateMenu(QString name)
{
    QMenu* test = new QMenu;
    test->setTitle("新建");
    test->addAction(QIcon(":/image/FileIcon/TxtFile.png"), QStringLiteral("文件"),this,SLOT(onCreateFile()));
    test->addAction(QIcon(":/image/FileIcon/dir.png"), QStringLiteral("文件夹"),this,SLOT(onCreateDir()));

    menu = new QMenu;
    menu->setTitle(name);
    //添加菜单项，指定图标、名称、响应函数
    menu->addMenu(test);
    menu->addAction(QIcon(":/image/FileIcon/WhiteF.png"), QStringLiteral("刷新"),this,SLOT(OnBscGroupRightAction()));
    menu->addAction(QIcon(":/image/FileIcon/WhiteF.png"), QStringLiteral("上传"),this,SLOT(onUpLoad()));
    menu->addAction(QIcon(":/image/FileIcon/Whitee.png"), QStringLiteral("下载"),this,SLOT(onDownLoad()));
    menu->setStyleSheet(
              "QMenu {\
                    background-color: white;\
                     border: 1px solid white;\
                }\
                QMenu::item {\
                     background-color: transparent;\
                     padding:8px 32px;\
                     margin:0px 8px;\
                     border-bottom:1px solid #DBDBDB;\
                }\
                QMenu::item:selected { \
                     background-color: #2dabf9;\
                }");
    test->setStyleSheet(
      "QMenu {\
            background-color: white;\
             border: 1px solid white;\
        }\
        QMenu::item {\
             background-color: transparent;\
             padding:8px 32px;\
             margin:0px 8px;\
             border-bottom:1px solid #DBDBDB;\
        }\
        QMenu::item:selected { \
             background-color: #2dabf9;\
        }");

        return menu;
}


//鼠标点击事件
void XMB::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton) //鼠标右键
    {
        menu = CreateMenu("file");
        menu->exec(QCursor::pos());
    }

}

void XMB::OnLast()
{
    QMessageBox::about(this,"a","last");
}
//刷新
void XMB::OnBscGroupRightAction()
{
    Init();
}

void XMB::onCreateFile()//新建文件
{

}
void XMB::onCreateDir()//新建文件夹
{

}
void XMB::onUpLoad()//上传
{
    MySocket sock;
    QString filename;
    filename = QFileDialog::getOpenFileName(this,"选择文件","d:\\","ALL Files(*.*);;Image Files(*.jpg *.png);;XML File(*.xml)");
    //QFile file(filename);
    //if( !file.open(QIODevice::Append | QIODevice::Text) )
     //   return;
    FILE* file = fopen(filename.toUtf8().data(),"rb");
    if(nullptr == file)
        return;

    std::string name;
    if(!getUsr(name))
        return;
    strcpy(com_tmp.Usr_name,name.c_str());
    com_tmp.com = MKDIR_FILE;
    strcpy(com_tmp.Path,"/");

    MyXml xml;
    xml.m_ComFile.push_back(com_tmp);
    xml.WriteXml(this,"./log/xml/upload.xml");

    if(!sock.InIt())
    {
        QMessageBox::warning(this,"error","bind error!");
        return;
    }
    if(!sock.ConnecttoHost())
    {
        QMessageBox::warning(this,"error","link error!");
        return;
    }

    Xml_Info info;
    int ret;
    char buff[MSGSIZE];
    FILE* fp = fopen("./log/xml/upload.xml","rb");
    if(fp == nullptr)
        return ;
    fseek(fp,0,SEEK_END);
    info.size = ftell(fp);
    strcpy(info.name,"upload.xml");
    rewind(fp);
    ret = sock.sendData(&info,sizeof(info));
    if(ret == sizeof(info))
    {
        while(!feof(fp))
        {
            ret = fread(buff,1,MSGSIZE,fp);
            sock.sendData(buff,ret);
        }     
    }
    else
    {
        fclose(fp);
        return;
    }
    fclose(fp);

    Xml_Info info1;
    strcpy(info1.name,pathName(filename.toUtf8().data()).c_str());
    fseek(file,0,SEEK_END);
    info1.size = ftell(file);
    rewind(file);
    ret = sock.sendData(&info1,sizeof(info1));
    if(ret == sizeof(info1))
    {
        //int t = 0;
        while(!feof(file))
        {
            ret = fread(buff,1,MSGSIZE,file);
            sock.sendData(buff,ret);
            //t += ret;
        }
    }
    fclose(file);
    sock.clear();
}

void XMB::onDownLoad()//下载
{

}

//closeEvent
void XMB::closeEvent(QCloseEvent* event)
{

   //跳出信息框，你是否要关闭.
   auto temp = QMessageBox::information(this, "tooltip", "你是否要关闭?", QMessageBox::Yes | QMessageBox::No);
        if (temp == QMessageBox::Yes)
        {
            // 接受了 要关闭这个窗口的事件. accept和ignore只是作为一个标志.
            //event->accept();
            event->ignore();
            this->hide();
        }
        else
        {
            //忽略了 要关闭这个窗口的事件.当前窗口就不会被关闭.
            event->ignore();
        }
}


void XMB::on_listWidget_itemActivated(QListWidgetItem *item)//双击
{

}

void XMB::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //static QToolButton* ButtonArry[MAXNUM];
    size_t i = 0;
    static QLabel* lab = nullptr;
    if(item == ui->listWidget->item(0)) //文件界面
    {
        if(!stat)
        for(i = 0; i < File_All.size();++i)
        {
            ButtonArry[i] = new QToolButton(ui->Mynet);
            ButtonArry[i]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            ButtonArry[i]->setIconSize(QSize(100,85));
            ButtonArry[i]->setStyleSheet("background-color: transparent;");
            if(File_All[i].FM == 0)
                ButtonArry[i]->setIcon(QIcon(":/image/FileIcon/dir.png"));
            else
                ButtonArry[i]->setIcon(QIcon(":/image/FileIcon/TxtFile.png"));
            ButtonArry[i]->setText(File_All[i].File_Name);
            if(i <= 4)
                ui->file_layout->addWidget(ButtonArry[i]);
            else if(i >= 5 && i<=9)
                ui->file_layout_2->addWidget(ButtonArry[i]);
            else
                ui->file_layout_3->addWidget(ButtonArry[i]);
            ButtonArry[i]->show();
        }
        stat = true;
    }
    else if(item == ui->listWidget->item(1))//图片界面
    {
        DELETEFILE(stat,File_All.size())

        VALUE_FALSE(stat)
    }
    else if(item == ui->listWidget->item(2))//音频界面
    {
        DELETEFILE(stat,File_All.size())
       // QMessageBox::about(this,"2","0");
        VALUE_FALSE(stat)
    }
    else if(item == ui->listWidget->item(3))//视频界面
    {
        DELETEFILE(stat,File_All.size())
        //QMessageBox::about(this,"3","0");
        VALUE_FALSE(stat)
    }
    else if(item == ui->listWidget->item(4))//其他界面
    {
        DELETEFILE(stat,File_All.size())
        //QMessageBox::about(this,"4","0");
        delete lab;
        VALUE_FALSE(stat)
    }
    else if(item == ui->listWidget->item(5))//关于界面
    {
        DELETEFILE(stat,File_All.size())
                lab = new QLabel(ui->Mynet);
                lab->setOpenExternalLinks(true);
                lab->setText("<a href=\"http://www.ityy520.com\">My Host");
                ui->file_layout->addWidget(lab);
        VALUE_FALSE(stat)
    }

}

//Find
void XMB::CreateWeb()
{
    lineUrl = new QLineEdit(ui->Find);
    lineUrl->setText("www.baidu.com");
    connect(lineUrl,SIGNAL(returnPressed()),this,SLOT(loadNavigate()));

    webWidget = new QAxWidget(ui->Find);
    //设置ActiveX控件为IEMicrosoft Web Browser
    //设置ActiveX控件的id，最有效的方式就是使用UUID
    //此处的{8856F961-340A-11D0-A96B-00C04FD705A2}就是Microsoft Web Browser控件的UUID
    webWidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    webWidget->setObjectName(QString::fromUtf8("webWidget"));//设置控件的名称
    webWidget->setFocusPolicy(Qt::StrongFocus);//设置控件接收键盘焦点的方式：鼠标单击、Tab键
    webWidget->setProperty("DisplayAlerts",false); //不显示任何警告信息。
    webWidget->setProperty("DisplayScrollBars",true); // 显示滚动条

    QVBoxLayout *mainLayout = new QVBoxLayout(ui->Find);
    mainLayout->addWidget(lineUrl);
    mainLayout->addWidget(webWidget);
    //setLayout(mainLayout);

    //setWindowState(Qt::WindowMaximized);//最大化
    loadNavigate();
}

void XMB::loadNavigate()
{
    QString sUrl = lineUrl->text().trimmed();
    webWidget->dynamicCall("Navigate(const QString&)",sUrl);
}
