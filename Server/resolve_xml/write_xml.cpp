/*************************************************************************
	> File Name: write_xml.cpp
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Fri 18 Jan 2019 11:24:44 AM CST
 ************************************************************************/

#include<iostream>
#include"write_xml.h"
using namespace std;
using namespace boost::property_tree;

int Write_Xml(vector<Data_Swap>&vt,string Path)
{
	vector<Data_Swap>::iterator it;
	ptree pt,File_Info,Data_Swap,entry;
	ptree pint,psize,ptime;

	for(it = vt.begin();it != vt.end(); ++it)
	{
		pint.put_value<int>(it->FM);
		ptime.put_value<string>(it->Finall_Time);
		psize.put_value<size_t>(it->File_Size);
		entry.put("<xmlattr>.File_Name",it->File_Name);
		entry.add_child("Finall_Time",ptime);
		entry.add_child("File_mode",pint);
		entry.add_child("File_Size",psize);
		Data_Swap.add_child("entry",entry);
		pint.clear();
		ptime.clear();
		psize.clear();
		entry.clear();
	}
	File_Info.add_child("Data_Swap",Data_Swap);
	pt.add_child("File_Info",File_Info);

	boost::property_tree::xml_writer_settings<string> settings = 
		boost::property_tree::xml_writer_make_settings<string>('\t',1);

	write_xml(Path,pt,std::locale(),settings);

	return 0;
}
/*
int main()
{
	vector<Data_Swap> a;
	Data_Swap tmp;
	strcpy(tmp.File_Name,"abc");
	strcpy(tmp.Finall_Time,"2018/1/1");
	tmp.FM = FILE_FILE;
	tmp.File_Size = 1232;
	a.push_back(tmp);
	a.push_back(tmp);
	a.push_back(tmp);

	Write_Xml(a,"xml/a.xml");

	return 0;
}*/
