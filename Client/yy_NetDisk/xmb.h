#ifndef XMB_H
#define XMB_H

#include <QDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QToolButton>
#include <iostream>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QAxWidget>
#include <QLineEdit>
#include <vector>
#include "qnavigationwidget.h"
#include "NetDisk_Type.h"

#define MAXNUM 20 //文件个数
namespace Ui {

    class XMB;
}

class XMB : public QDialog
{
    Q_OBJECT

public:
    explicit XMB(QWidget *parent = 0);
    ~XMB();

private:
    void Init();//初始化
    void mMenu();//初始化菜单栏
    QMenu* CreateMenu(QString); //创建菜单
    void CreateWeb(); //创建浏览器窗口

    bool getUsr(std::string&);//获取用户名

private slots:

    void OnLast();//后退
    void OnBscGroupRightAction();//刷新
    void onCreateFile();//新建文件
    void onCreateDir();//新建文件夹
    void onUpLoad();//上传
    void onDownLoad();//下载
    void loadNavigate();

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::XMB *ui;
    Data_Swap file_tmp; //文件信息结构体
    Com_File com_tmp; //命令信息结构体
    QToolButton* ButtonArry[MAXNUM];
    QMenuBar* MyMenuBar;//菜单栏
    QMenu* menu;//菜单
    QLineEdit *lineUrl; //url栏目
    QAxWidget* webWidget; //浏览器窗口
    std::vector<Data_Swap> File_All; //所有文件信息


protected:
     void mousePressEvent(QMouseEvent*);
     //这是一个虚函数，继承自QEvent.只要重写了这个虚函数，当你按下窗口右上角的"×"时，就会调用你所重写的此函数.
     void closeEvent(QCloseEvent*event);

};
//从全路径中获取文件名
inline std::string pathName(std::string name)
{
    size_t index;
    for(index = name.size() - 1;index >= 0; --index)
    {
        if(name[index] == '/')
            break;
    }
    if(index < 0)
        return name;
    else
        return name.substr(index + 1,name.size() - index);
}

#endif // XMB_H
