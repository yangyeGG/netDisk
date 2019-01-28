/*************************************************************************
	> File Name: write_xml.h
	> Author: yy
	> Mail: yy@ityy520.com 
	> Created Time: Fri 18 Jan 2019 11:12:47 AM CST
 ************************************************************************/

#ifndef _WRITE_XML_H_
#define _WRITE_XML_H_
#include<iostream>
#include<vector>
#include<string>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
#include<boost/foreach.hpp>
#include"NetDisk_Type.h"
using namespace std;
using namespace boost::property_tree;

int Write_Xml(vector<Data_Swap>&,string);

#endif
