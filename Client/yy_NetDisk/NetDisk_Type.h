#ifndef NETDISK_TYPE_H
#define NETDISK_TYPE_H
#include <string>
#include <cstring>
#include "mynet_def.h"
enum Mode{
    MAIL_LOGIN = 0,//短信登录
    LOGN_IN = 1,//普通登录
    LOGN_UP = 2 // 注册
};

enum Commit
{
    INIT_FILE = 0,//初始化
    MKDIR_FILE = 1,//新建文件
    DEL_FILE = 2, //删除文件
    CHANGE_FILE = 3 //修改文件
};

enum File_mode
{
    DIR_FILE = 0, //文件夹
    FILE_FILE = 1,//普通文件
    MUSIC_FILE = 2,//音乐文件
    MOVIE_FILE = 3,//视频文件
    PICTURE_FILE = 4//图片
};

enum ErrorCode
{
    NET_ERROR, //网络异常
    FILEOPEN_FAIL, //文件打开失败
    BIND_ERROR, //绑定失败
    GET_FILEINFO_FAIL, //获取文件信息失败
    SENDDATA_FAIL, //发送数据失败
    RECVDATA_FAIL //接收数据失败
};

#pragma pack(1)
typedef struct _usr_info //用户信息
{
    Mode e_mode;
 //   bool mode;
    char _usr[20];
    char Passwd[33];
    char AuthCode[5];
}usr_info;


typedef struct authcode_ //用户验证码
{
    char _Usr[20];

}authcode;

typedef struct Data_Swap_ //文件数据
{
    char File_Name[30];
    char Finall_Time[20];
    File_mode FM;
    unsigned long File_Size;

    Data_Swap_(std::string name_, std::string time_, int fm_,unsigned long size_)
    {
        strcpy(File_Name,name_.c_str());
        strcpy(Finall_Time,time_.c_str());
        FM = (File_mode)fm_;
        File_Size = size_;
    }
    Data_Swap_(){}
}Data_Swap;

typedef struct Com_File_ //命令数据
{
    char Usr_name[20];
    Commit com;
    char Path[40];

    Com_File_(std::string name_, int fm_,std::string size_)
    {
        strcpy(Usr_name,name_.c_str());
        strcpy(Path,size_.c_str());
        com= (Commit)fm_;

    }
    Com_File_(){}
}Com_File;

typedef struct File_Num_ //文件个数
{
    int file_num;
}File_Num;

typedef struct Xml_Info_ //xml文件信息
{
    char name[51];
    long long size;
}Xml_Info;

#pragma pack()

#endif // NETDISK_TYPE_H
