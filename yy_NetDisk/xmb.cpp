#include "xmb.h"
#include "ui_xmb.h"

XMB::XMB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XMB)
{
    ui->setupUi(this);
}

XMB::~XMB()
{
    delete ui;
}
