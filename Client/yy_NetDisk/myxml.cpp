#include "myxml.h"
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <QFileDialog>
#include <QTextStream>
USING_NAMESPACE_MYNET_XML
MyXml::MyXml()
{

}

bool MyXml::readFile(const QString &fileName)
{
    m_DataSwap.clear();
    m_ComFile.clear();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "File_Info")
    {
        std::cerr << "Error: Not a File_Info file" << std::endl;
        return false;
    }

    parseFileInfoMembers(root);
    return true;
}

void MyXml::printAllMembers()
{
    std::cout << "All DataSwap: " << std::endl;
    for (const auto& DataSwap : m_DataSwap)
    {
        print1(std::cout, DataSwap);
    }
    std::cout << "All ComFile: " << std::endl;
    for (const auto& ComFile : m_ComFile)
    {
        print1(std::cout, ComFile);
    }
}


void MyXml::parseFileInfoMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        if (child.toElement().tagName() == "Data_Swap")
        {
            parseDataSwapMembers(child.toElement());
        }
        else if (child.toElement().tagName() == "Com_File")
        {
            parseComFileMembers(child.toElement());
        }
        child = child.nextSibling();
    }
}

void MyXml::parseDataSwapMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        if (child.toElement().tagName() == "entry")
        {
            parseDataSwap(child.toElement());
        }
        child = child.nextSibling();
    }
}

void MyXml::parseComFileMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        if (child.toElement().tagName() == "entry")
        {
            parseComFile(child.toElement());
        }
        child = child.nextSibling();
    }
}

void MyXml::parseDataSwap(const QDomElement &element)
{
    auto children = element.childNodes();
    auto firstChild = children.at(0).toElement();
    auto secondChild = children.at(1).toElement();
    auto thridChild = children.at(2).toElement();
    int FM = secondChild.text().toInt();
    unsigned long size = thridChild.text().toULong();

    //m_DataSwap.emplace_back(element.attribute("name").toStdString(),
     //           age, secondChild.text().toStdString());
    m_DataSwap.emplace_back(element.attribute("File_Name").toStdString().c_str(),
                            firstChild.text().toStdString().c_str(),
                            FM,size);
}

void MyXml::parseComFile(const QDomElement &element)
{
    auto children = element.childNodes();
    auto firstChild = children.at(0).toElement();
    auto secondChild = children.at(1).toElement();
    int com = firstChild.text().toInt();

    m_ComFile.emplace_back(element.attribute("Usr_name").toStdString().c_str(),
                com, secondChild.text().toStdString().c_str());
}


bool MyXml::WriteXml(QWidget* This ,QString filename)
{
   // QString filename = QFileDialog::getSaveFileName( This, "Save File", "./log/xml/data.xml", "*.xml" );

    QFile file( filename );
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        return false;
    }

    QDomDocument document;

    QString strHeader( "version=\"1.0\" encoding=\"utf-8\"" );
    document.appendChild( document.createProcessingInstruction("xml", strHeader) );

    QDomElement	root_elem = document.createElement( "File_Info" );//主根
    document.appendChild( root_elem );


    //文件结构体
    for(std::vector<Data_Swap_>::iterator it = m_DataSwap.begin();it != m_DataSwap.end(); ++it)
    {
        QDomElement item_Data_Swap = document.createElement( "Data_Swap" );
        root_elem.appendChild( item_Data_Swap );

        QDomElement item_entry = document.createElement( "entry" );
        item_entry.setAttribute("File_Name",it->File_Name);
        item_Data_Swap.appendChild(item_entry);

        QDomElement item_Finall_Time = document.createElement( "Finall_Time" );
        item_entry.appendChild(item_Finall_Time);

        auto a = document.createTextNode(it->Finall_Time);
        item_Finall_Time.appendChild(a);



        QDomElement item_File_mode = document.createElement( "File_mode" );
        item_entry.appendChild(item_File_mode);

        std::string fm = (char*)it->FM;
        auto b = document.createTextNode(fm.c_str());
        item_File_mode.appendChild(b);


        QDomElement item_File_Size = document.createElement( "File_Size" );
        item_entry.appendChild(item_File_Size);

        auto c = document.createTextNode((char*)(it->File_Size));
        item_File_Size.appendChild(c);
    }


    for(std::vector<Com_File_>::iterator it = m_ComFile.begin();it != m_ComFile.end();++it)
    {
        //命令结构体
        QDomElement item_Com_File = document.createElement( "Com_File" );
        root_elem.appendChild( item_Com_File );

        QDomElement item_entry1 = document.createElement( "entry" );
        item_entry1.setAttribute("Usr_name",it->Usr_name);
        item_Com_File.appendChild(item_entry1);

        QDomElement item_com = document.createElement( "com" );
        item_entry1.appendChild(item_com);
        char buff[10];
        sprintf(buff,"%d",it->com);
        auto e = document.createTextNode(buff);
        item_com.appendChild(e);



        QDomElement item_Path = document.createElement( "Path" );
        item_entry1.appendChild(item_Path);

        auto f = document.createTextNode(it->Path);
        item_Path.appendChild(f);
    }

    QTextStream out( &file );
    document.save( out, 9 );
    file.close();

    return true;
}
