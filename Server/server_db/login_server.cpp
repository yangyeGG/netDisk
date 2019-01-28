/*************************************************************************
    > File Name: login_service.cpp
    > Author: yy
    > Mail: yy@ityy520.com 
    > Created Time: Wed 21 Nov 2018 08:14:06 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#include "../NetDisk_db/MyDB.h"
#include "NetDisk_Type.h"

unsigned char authcode[5] = {
	'0','0','0','0','\0'
};//短信验证码

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
	myaddr.sin_port = htons(11111);//端口号

	if(bind(sock_listen,(struct sockaddr*)&myaddr,sizeof(myaddr)))//绑定
	{
		perror("bind error");
		exit(1);
	}

	listen(sock_listen,10);
	printf("登录服务启动成功，运行中!\n");
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
	
	usr_info info;//用户信息
	authcode1 auth_code;//验证码请求，手机号码

	int ret;
	ret = recv(sock_conn,&info,sizeof(info),0);
	std::cout<<info.Usr<<" "<<info.passwd<<std::endl;
	printf("ret = %d\n",ret);
	if(ret == sizeof(authcode1))//发送短信验证码
	{
		recv(sock_conn,&auth_code,sizeof(auth_code),0);
		for(int i = 0; i < 4;++i)
			authcode[i]=rand()%10 + '0';
		char buff[250];
		sprintf(buff,"python ../dysms_python/dysms_python/send.py %s \"{\\\"code\\\":\\\"%s\\\",\\\"product\\\":\\\"ytx\\\"}\"",auth_code._Usr,authcode);
		std::cout<<buff<<std::endl;
		system(buff);
	}
	else if(ret == sizeof(usr_info))
	{
		MyDB db;
		db.initDB("localhost","root","","yy");
		if(info.e_mode == LOGN_IN)//普通登录
		{			
			//数据库校验
			int tmp = db.checkout(info.Usr,info.passwd);
			printf("tmp = %d\n",tmp);
			if(tmp == 1)
			{
				//用户名不存在
				char buff[]="No Usr!";
				ret = send(sock_conn,&buff,sizeof(buff),0);
				std::cout<<buff<<std::endl;
				printf("tmp = %d\n",ret);
			}
			else if(tmp == 0)
			{
				//用户密码正确
				char buff[]="success!";
				ret = send(sock_conn,&buff,sizeof(buff),0);
				std::cout<<buff<<std::endl;
				printf("ret = %d\n",ret);
			}
			else if(tmp == -1)
			{
				//用户名或密码错误
				char buff[]="Usr or passwd error!";
				ret = send(sock_conn,&buff,sizeof(buff),0);
				std::cout<<buff<<std::endl;
				printf("ret = %d\n",ret);
			}
		}
		else if(info.e_mode == MAIL_LOGIN)//短信登录
		{
			int tmp = db.checkout(info.Usr,info.passwd);
			printf("tmp = %d\n",tmp);
			if(tmp == 1)
			{
				//用户名不存在
				char buff[]="No Usr!";
				ret = send(sock_conn,&buff,sizeof(buff),0);
				std::cout<<buff<<std::endl;
				printf("tmp = %d\n",ret);
			}
			else 
			{
				//用户存在

				if(strcmp(info.passwd,(const char*)authcode)==0)
				{
					char buff[]="success!";
					ret = send(sock_conn,&buff,sizeof(buff),0);
					std::cout<<buff<<std::endl;
				}
				else
				{
					char buff[]="Auth_code error!";
					ret = send(sock_conn,&buff,sizeof(buff),0);
					std::cout<<buff<<std::endl;
					printf("ret = %d\n",ret);
				}
			}	
		}
		else if(info.e_mode == LOGN_UP)//注册
		{
			//判断验证码是否正确
			if(strcmp(info.AuthCode,(const char*)authcode) == 0)
			{
				int tmp	= db.Insert_DB(info.Usr,info.passwd);
				if(0 == tmp)
				{
					char buff[] = "sign_up success!";
					ret = send(sock_conn,&buff,sizeof(buff),0);
					char commit[50];
					sprintf(commit,"mkdir /root/NetDisk_Usr_Dir/%s",info.Usr);
					system(commit);
					std::cout<<buff<<std::endl;
				}
				else if(1 == tmp)
				{
					char buff[] = "Usr exists!";
					ret = send(sock_conn,&buff,sizeof(buff),0);
					std::cout<<buff<<std::endl;
				}
			}
			else
			{
				char buff[] = "sign_up Auth_code error!";
				ret = send(sock_conn,&buff,sizeof(buff),0);
				std::cout<<buff<<std::endl;
			}
		}

	}
	
	printf("\n客户端%s:%d已经下线\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

}
