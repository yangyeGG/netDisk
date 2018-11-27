#include <QtGui/QApplication>
#include "login.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QApplication a(argc, argv);
    login w;
    w.show();


    return a.exec();
}
