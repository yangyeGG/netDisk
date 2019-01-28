/*************************************************************************
	> File Name: read_xml.cpp
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Wed 09 Jan 2019 09:27:29 AM CST
 ************************************************************************/

#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
#include<boost/typeof/typeof.hpp>
#include"NetDisk_Type.h"
using namespace std;
using namespace boost::property_tree;

int readXml(vector<Data_Swap_>& vData,vector<Com_File_>& vFile,
		string path)
{
	ptree pt,pData,pFile;
	
	try
	{
		read_xml(path,pt);
		pData = pt.get_child("File_Info.Data_Swap");
	}
	catch(exception& e)
	{
		//错误码未写
	}

	for(ptree::iterator it = pData.begin();
			it!= pData.end();it++)
	{
		ptree entry = it->second;
		Data_Swap_ node;
		strcpy(node.File_Name,entry.get<string>
				("<xmlattr>.File_Name"," error").c_str());
		strcpy(node.Finall_Time,entry.get<string>
				("Finall_Time","error").c_str());
		node.FM = (File_mode) entry.get<int>("File_mode",0);
		node.File_Size = entry.get<unsigned long>("File_Size",0);
		vData.push_back(node);
	}

	try
	{
		pFile = pt.get_child("File_Info.Com_File");	
	}
	catch(exception& q)
	{
		return 2;
	}
	for(ptree::iterator it = pFile.begin();
			it != pFile.end(); ++it)
	{
		ptree entry = it->second;
		Com_File_ node;
		strcpy(node.Usr_name,entry.get<string>
				("<xmlattr>.Usr_name","error").c_str());
		node.com = (Commit) entry.get<int>("com",0);
		strcpy(node.Path,entry.get<string>
				("Path","error").c_str());
		vFile.push_back(node);
	}
	return 0;
}





