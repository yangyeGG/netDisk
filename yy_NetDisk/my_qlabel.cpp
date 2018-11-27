#include "my_qlabel.h"

my_QLabel::my_QLabel(QWidget *parent) :
    QLabel(parent)
{
    setText("test");
    setFixedSize(100,100);
}
