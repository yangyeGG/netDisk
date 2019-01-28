#ifndef MYXML_H
#define MYXML_H
#include "NetDisk_Type.h"
#include <QString>
#include <qwidget.h>
#include <QDomElement>

NAMESPACE_MYNET_XML
inline void print1(std::ostream &out, const Data_Swap& DataSwap)
{
    out << "DataSwap: " << DataSwap.File_Name << std::endl;
    out << "\ttime: " << DataSwap.Finall_Time << std::endl;
    out << "\tFM: " << DataSwap.FM << std::endl;
    out << "\tSIZE: " << DataSwap.File_Size << std::endl;
}

inline void print1(std::ostream& out, const Com_File& ComFile)
{
    out << "ComFile: " << ComFile.Usr_name << std::endl;
    out << "\tcom: " << ComFile.com << std::endl;
    out << "\tpath: " << ComFile.Path << std::endl;
}

class MyXml
{
public:
    MyXml();

    bool readFile(const QString &fileName);
    void printAllMembers();
    bool WriteXml(QWidget* This,QString);

private:
        void parseFileInfoMembers(const QDomElement &element);
        void parseDataSwapMembers(const QDomElement &element);
        void parseComFileMembers(const QDomElement &element);
        void parseDataSwap(const QDomElement &element);
        void parseComFile(const QDomElement &element);
public:
        std::vector<Data_Swap>  m_DataSwap;
        std::vector<Com_File>  m_ComFile;
};

NAMESAPCE_END
#endif // MYXML_H
