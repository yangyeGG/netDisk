#include "logon.h"
#include "ui_logon.h"
#include <QDesktopServices>
#include <QUrl>

logon::logon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logon)
{
    ui->setupUi(this);
}

logon::~logon()
{
    delete ui;
}

void logon::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("www.baidu.com"));
}
