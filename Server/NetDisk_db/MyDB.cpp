#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "MyDB.h"
using namespace std;

MyDB::MyDB()
{
	connection = mysql_init(NULL);//初始化数据库
	if(connection == NULL)
	{
		cout << "Error1:" << mysql_error(connection);
		exit(1);
	}
}

MyDB::~MyDB()
{
	if(connection != NULL)
	{
		mysql_close(connection);//关闭数据库
	}
}

bool MyDB::initDB(string host,string user,string pwd, string db_name)
{
	//函数mysql_real_connect建立一个数据库连接
	//成功返回MYSQL* 连接句柄，失败返回NULL
	if(mysql_real_connect(connection, host.c_str(),user.c_str(),
					pwd.c_str(), db_name.c_str(), 0, NULL ,0)== NULL)
	{
		
		cout<< "Error2:" << mysql_error(connection)
			<<endl;
		fprintf(stderr,"Failed to connect to database Error:%s\n",connection);
		exit(1);
	}
	mysql_query(connection,"set names utf8");
	return true;
}

bool MyDB::exeSQL(string sql)
{
	//mysql_query()执行成功返回0，失败返回非0值
	if(mysql_query(connection,sql.c_str()))
	{
		cout << "Query Error:" << mysql_error(connection);
		exit(1);
	}
	else
	{
		result = mysql_use_result(connection);//获取结果集
		unsigned int column=mysql_field_count(connection); //返回connection查询的列数
	//	for(int i=0;i<column;++i)
		while(result)
		{
			//获取下一行
			row = mysql_fetch_row(result);
			if(row <= 0)
			{
				break;
			}
			//mysql_num_fields()返回结果集中的字段数
			for(int j= 0; j< mysql_num_fields(result); ++j)
			{
				cout<< row[j] <<" ";
			}
			cout<< endl;
		}
		//释放结果集的内存
		mysql_free_result(result);
	}
	return true;
}


int MyDB::checkout(string usr,string Passwd)
{
	string sql="select Usr,passwd from Usr_info where Usr ='" + usr + "';";
//	cout<<"sql = "<<sql<<endl;
	if(mysql_query(connection,sql.c_str()))
	{
		cout << "Query Error:" << mysql_error(connection);
		mysql_free_result(result);
		return -5;
	}
	else
	{
		result = mysql_store_result(connection);
		row = mysql_fetch_row(result);
		if(row == NULL)
		{
			mysql_free_result(result);
			return 1; //用户名不存在
		}
		if(usr == row[0])
		{
			if(Passwd == row[1])
			{
				mysql_free_result(result);
				return 0; //用户密码正确
			}
			else
			{
				mysql_free_result(result);
				return -1;//用户名或密码错误
			}
		}
	}

}

int MyDB::Insert_DB(string usr,string Passwd)
{
	string sql = "select Usr from Usr_info where Usr ='" + usr + "';";
	if(mysql_query(connection,sql.c_str()))
	{
		cout<< "Query Error:" << mysql_error(connection);
		mysql_free_result(result);
		return -5;//异常返回
	}
	else
	{
		result = mysql_store_result(connection);
		row = mysql_fetch_row(result);
		if(row == NULL)//插入数据库
		{
			string Insert_Sql = "insert into Usr_info(Usr,passwd) values('" + usr + "','" + Passwd + "');";
			if(mysql_query(connection,Insert_Sql.c_str()))
			{
				cout<< "Query Error:" << mysql_error(connection);
				mysql_free_result(result);
				return -4;//异常返回				
			}
			mysql_free_result(result);
			return 0;//插入成功
		}
		else
		{
			mysql_free_result(result);
			return 1; //用户名存在
		}
	
	}


}







