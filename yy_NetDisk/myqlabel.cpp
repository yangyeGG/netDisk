#include "myqlabel.h"

MyQLabel::MyQLabel(QObject *parent):
    QLabel(parent)
{
    //setFixedSize(50,50);

}

void MyQLabel::enterEvent(QEvent * e)
{
    setStyleSheet("color:blue");
}

void MyQLabel::leaveEvent(QEvent * e)
{
    setStyleSheet("color:black");
}

