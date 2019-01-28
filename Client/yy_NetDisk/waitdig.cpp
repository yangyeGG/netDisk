#include "waitdig.h"
#include "ui_waitdig.h"
#include <QDesktopWidget>
#include <QMovie>

WaitDig::WaitDig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDig)
{
    ui->setupUi(this);

    setWindowModality(Qt::WindowModal);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);//ÎÞ±ß¿ò
    setAttribute(Qt::WA_TranslucentBackground);//±³¾°Í¸Ã÷
    //ÆÁÄ»¾çÖÐ
    setWindowTitle("Loading");
    int frmX = width();
    int frmY = height();

    QDesktopWidget w;

    int deskWidth = w.width();
    int deskHeight = w.height();

    QPoint movePoint(deskWidth / 2 - frmX / 2 , deskHeight / 2 - frmY / 2);

    move(movePoint);

    //¼ÓÔØGIFÍ¼Æ¬
    QMovie *movie = new QMovie(":/image/loading.gif");
    ui->label->setMovie(movie);

    movie->start();

}

WaitDig::~WaitDig()
{
    delete ui;
}


