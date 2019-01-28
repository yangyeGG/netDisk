/*************************************************************************
    > File Name: main_service.cpp
    > Author: yy
    > Mail: yy@ityy520.com 
    > Created Time: Wed 21 Nov 2018 08:14:06 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#include<dirent.h>
#include<sys/stat.h>
#include<iostream>
#include<fstream>
#include "NetDisk_Type.h"
#include "mylib.h"
#include "../resolve_xml/read_xml.h"
#include "../resolve_xml/write_xml.h"
using namespace std;

Data_Swap Tmp_Data;//数据传输
Com_File Com_Data;//指令数据
File_Num Num; //文件个数
vector<Data_Swap> File_info;

void* thr_login(void* arg);

int main()
{
	srand(time(NULL));
	int sock_listen;

	sock_listen = socket(AF_INET,SOCK_STREAM,0);//监听套I接字

	int optval = 1;
	setsockopt(sock_listen,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(11112);//端口号

	if(bind(sock_listen,(struct sockaddr*)&myaddr,sizeof(myaddr)))//绑定
	{
		perror("bind error");
		exit(1);
	}

	listen(sock_listen,10);
	printf("主服务启动成功，运行中!\n");
	struct sockaddr_in cliaddr;
	socklen_t len;

	while(1)
	{
		int sock_conn;//连接套接字

		signal(SIGPIPE,SIG_IGN);
		len = sizeof(cliaddr);
		sock_conn = accept(sock_listen,(struct sockaddr*)&cliaddr,&len);

		if(sock_conn == -1)
		{
			perror("accept error");
			continue;
		}


		pthread_t tid;
		if(pthread_create(&tid,NULL,thr_login,(void*)(long)sock_conn))
		{
			perror("create new thread fail");
			close(sock_conn);
			continue;
		}
	}

	close(sock_listen);

	return 0;
}

//初始化
void init(int sock_conn,Com_File& tmp)
{
	char path[100];
	sprintf(path,"/root/NetDisk_Usr_Dir/%s",tmp.Usr_name);
	Num.file_num = traversal_dir(path,File_info);
	cout<<"file_num = "<<Num.file_num<<endl;
	int ret = 0;

		cout<<File_info[0].File_Name<<endl;
		ret = Write_Xml(File_info,"./log/xml/send.xml");
		cout<<"xml success!"<<endl;
		Xml_Info info;
		char buff[1024];
		FILE* fp = fopen("./log/xml/send.xml","rb");
		if(NULL == fp)
		{
			cout<<"文件不存在\n";
			return;
		}
			
		fseek(fp,0,SEEK_END);
		info.size = ftell(fp);
		strcpy(info.name,"send.xml");
		rewind(fp);
		ret = send(sock_conn,&info,sizeof(info),0);
		if(sizeof(info) == ret)
		{
			//cout<<"1\n";
			while(!feof(fp))
			{
				ret = fread(buff,1,1024,fp);
				cout<<"fread ret = "<<ret<<endl;
				cout<<write(sock_conn,buff,ret)<<endl;
			}
		}
		fclose(fp);

}

//上传文件
void UpLoad(int sock,Com_File& tmp)
{
	char path[100];
	char buff[1024];
	char filename[100];
	sprintf(path,"/root/NetDisk_Usr_Dir/%s%s",tmp.Usr_name,tmp.Path);
//	std::cout<<path<<endl;
	Xml_Info info;
	
	int ret = recv(sock,&info,sizeof(info),0);
	if(sizeof(info) == ret)
	{
		//std::cout<<info.name<<std::endl;
		sprintf(filename,"%s%s",path,info.name);
		std::cout<<filename<<std::endl;
		FILE* fp = fopen(filename,"wb");
		if(NULL == fp)
		{
			//error = ;
			std::cout<<"error\n";
			return;
		}
		int tmp = 0;
		std::cout<<"file size:"<<info.size<<std::endl;
		while(ret > 0)
		{
			ret = recv(sock,&buff,1024,0);
			fwrite(buff,1,ret,fp);
			//tmp += ret;
			cout<<ret<<endl;
		}	
		fclose(fp);
	}

}

//线程函数
void* thr_login(void* arg)
{
	pthread_detach(pthread_self());//线程结束自动释放空间

	int sock_conn = (long)arg; //传入套接字
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	getpeername(sock_conn,(struct sockaddr*)&cliaddr,&len);

	printf("\n客户端%s:%d已经连接!\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
	
	struct timeval timeout={2,0};//设置链接套接字超时校验2S
	setsockopt(sock_conn,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	setsockopt(sock_conn,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	

	Xml_Info xmlinfo;
	char buff[1024];
	char filename[1024];
	int ret;
	ret = recv(sock_conn,&xmlinfo,sizeof(Xml_Info),0);
	cout<<"Xml_Info ret = "<<ret<<endl;

	sprintf(filename,"./log/xml/%s",xmlinfo.name);
	std::cout<<filename<<std::endl;

	if(sizeof(Xml_Info) == ret)
	{
		ofstream fi(filename,std::ofstream::out);
		int t = 0;
		while(t != xmlinfo.size)
		{
			ret = recv(sock_conn,buff,1,0);
			t += ret;
			fi.write(buff,ret);
		}
		fi.close();

	}
	//读取XML信息
	vector<Data_Swap> tmp1;
	vector<Com_File> tmp2;

	readXml(tmp1,tmp2,filename);

	
	int Etmp = tmp2[0].com;
	switch(Etmp)
	{
		case INIT_FILE: // 初始化
		{
			init(sock_conn,tmp2[0]);
		}
		break;
		//上传文件
		case MKDIR_FILE:
		{
			UpLoad(sock_conn,tmp2[0]);
		}
		break;
	}
	
	printf("\n客户端%s:%d已经下线\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

}

