/*************************************************************************
	> File Name: NetDisk_Type.h
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Thu 20 Dec 2018 08:09:09 PM CST
 ************************************************************************/

#ifndef _NETDISK_TYPE_H_
#define _NETDISK_TYPE_H_
#include <string>
using namespace std;


enum Mode
{
	MAIL_LOGIN = 0, //短信登录
	LOGN_IN = 1, //普通登录
	LOGN_UP = 2 //注册
};

enum Commit
{
	INIT_FILE = 0, //初始化
	MKDIR_FILE = 1, //新建文件
	DEL_FILE = 2, // 删除文件
	CHANGE_FILE = 3 //修改文件

};

enum File_mode
{
	DIR_FILE = 0,//文件夹
	FILE_FILE = 1,//普通文件
	MUSIC_FILE = 2,//音乐文件
	MOVIE_FILE = 3,//视频文件
	PICTURE_FILE = 4 //图片
};
#pragma pack(1)
typedef struct _usr_info //用户信息
{
	Mode e_mode;
	 //bool mode;//true 普通登录 false 短信登录
	 char Usr[20];
	 char passwd[33];
	 char AuthCode[5];
}usr_info;

typedef struct authcode_  // 验证码
{
	char _Usr[20];
}authcode1;

typedef struct Data_Swap_ // 数据交换
{	
	char File_Name[30];
	char Finall_Time[20];
	File_mode FM;
	unsigned long File_Size;
}Data_Swap;

typedef struct Com_File_//命令操作
{
	char Usr_name[20];
	Commit com;
	char Path[40];
}Com_File;

typedef struct File_Num_ //文件个数
{
	int file_num;
}File_Num;
#pragma pack()

#endif
