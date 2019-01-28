/*************************************************************************
	> File Name: mylib.cpp
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Fri 18 Jan 2019 04:01:32 PM CST
 ************************************************************************/

#include<iostream>
#include"mylib.h"
using namespace std;


//遍历文件夹
int traversal_dir(string Path,vector<Data_Swap>& File_info)
{
	File_info.clear();
	struct stat st;
	DIR* dirp = opendir(Path.c_str());
	int cnt = 0;

	if(dirp == NULL)
	{
		perror("opendir");
		return -1;
	}

	struct dirent* pde = NULL;

	char old_cwd[50];
	getcwd(old_cwd,sizeof(old_cwd));
	chdir(Path.c_str());

	while(pde = readdir(dirp))
	{
	//	printf("%s\n",pde->d_name);
		if(stat(pde->d_name,&st) == 0)
		{
			if(S_ISDIR(st.st_mode))
			{
				//文件夹
				if(strcmp(pde->d_name,".")!=0 && strcmp(pde->d_name,"..")!=0)
				{	
					Data_Swap tmp;
					tmp.FM = DIR_FILE;
					//tmp.File_Name = pde->d_name;
					strcpy(tmp.File_Name,pde->d_name);
					tmp.File_Size = st.st_size;
					//tmp.Finall_Time = st.st_atime;
					char buf[10];
					sprintf(buf,"%ld",st.st_atime);
					strcpy(tmp.Finall_Time,buf);
					File_info.push_back(tmp);
					++cnt;
					//test
					cout<<"file_mode:"<<tmp.FM<<endl;
					cout<<"file_name:"<<tmp.File_Name<<endl;
					cout<<"file_size:"<<tmp.File_Size<<endl;
					cout<<"file_time:"<<tmp.Finall_Time<<endl;
				}
			}
			else  //普通文件
			{
			
				Data_Swap tmp;
				tmp.FM = FILE_FILE; //文件类型
			//	tmp.File_Name = pde->d_name; //文件名
				strcpy(tmp.File_Name,pde->d_name);
				tmp.File_Size = st.st_size; //文件大小
				//tmp.Finall_Time = st.st_atime; //最后一次访问时间
				char buf[10];
				sprintf(buf,"%ld",st.st_atime);
				strcpy(tmp.Finall_Time,buf);

				File_info.push_back(tmp);
				++cnt;
					//test
					cout<<tmp.FM<<endl;
					cout<<tmp.File_Name<<endl;
					cout<<tmp.File_Size<<endl;
					cout<<tmp.Finall_Time<<endl;
			}
		}
		
	}
	//std::cout<<"lao lujing "<<old_cwd<<endl;
	chdir(old_cwd);
	
	closedir(dirp);

	return cnt;

}
