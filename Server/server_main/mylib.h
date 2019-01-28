/*************************************************************************
	> File Name: mylib.h
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Fri 18 Jan 2019 03:56:39 PM CST
 ************************************************************************/
#ifndef _MYLIB_H_
#define _MYLIB_H_
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include "NetDisk_Type.h"
/*功能：遍历指定目录下的所有文件和文件夹，将文件信息存入向量中
 *第一个参数：文件路径（相对或绝对）
 *第二个参数：新建存放文件信息结构体的向量
 *返回值：成功返回文件或文件夹的个数 
 *
 */
int traversal_dir(string,vector<Data_Swap>&);

#endif

