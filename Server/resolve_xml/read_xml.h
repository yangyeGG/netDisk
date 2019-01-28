/*************************************************************************
	> File Name: read_xml.h
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Wed 09 Jan 2019 09:43:06 AM CST
 ************************************************************************/

#ifndef _READ_XML_H_
#define _READ_XML_H_
#include<vector>
#include<string>
#include"NetDisk_Type.h"

int readXml(vector<Data_Swap_>& ,vector<Com_File_>&,string);

inline void printData(vector<Data_Swap>& vt)
{
	for(size_t i = 0; i < vt.size(); ++i)
	{
		std::cout<<vt[i].File_Name<<std::endl
			<<vt[i].Finall_Time<<std::endl
			<<vt[i].FM<<std::endl
			<<vt[i].File_Size<<std::endl;
	}
}

inline void printFile(vector<Com_File>& vt)
{
	for(size_t i =0;i<vt.size();++i)
	{
		std::cout<<vt[i].Usr_name<<std::endl
			<<vt[i].com<<std::endl
			<<vt[i].Path<<std::endl;
	}
}
#endif

