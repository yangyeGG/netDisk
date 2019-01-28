/*************************************************************************
	> File Name: testmain.cpp
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Wed 09 Jan 2019 02:07:46 PM CST
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include"NetDisk_Type.h"
#include"read_xml.h"
using namespace std;
/*
void printData(vector<Data_Swap>& vt)
{
	for(size_t i=0;i<vt.size();++i)
	{
		cout<<vt[i].File_Name<<endl
			<<vt[i].Finall_Time<<endl
			<<vt[i].FM<<endl
			<<vt[i].File_Size<<endl;
	}
}

void printFile(vector<Com_File>& vt)
{
	for(size_t i=0;i<vt.size();++i)
	{
		cout<<vt[i].Usr_name<<endl
			<<vt[i].com<<endl
			<<vt[i].Path<<endl;
	}
}
*/
int main()
{
	vector<Data_Swap> dataSwap;
	vector<Com_File> comFile;
	string xmlPath = "xml/a.xml";
	
	readXml(dataSwap,comFile,xmlPath);

	printData(dataSwap);
	printFile(comFile);
	return 0;
}

