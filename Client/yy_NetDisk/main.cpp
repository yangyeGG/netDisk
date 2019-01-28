#include <QApplication>
#include "login.h"
#include "xmb.h"

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QApplication a(argc, argv);
    login w;
    w.show();

    //XMB w;
    //w.show();

    return a.exec();
}

