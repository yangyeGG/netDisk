#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QString>
#include <QMouseEvent>
#include <QTcpSocket>
#include <QTimer>
//#include "my_qlabel.h"

namespace Ui {
    class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private:
    Ui::login *ui;
    QTcpSocket* socket;
    QTimer* timer;




private slots:


    void on_auth_code_clicked();
    void on_Login_clicked();
    void socket_connect();
    void socket_error();
    void recv_data();
    void update();

protected:
     void mousePressEvent(QMouseEvent*);
     void mouseMoveEvent(QMouseEvent *);

 };

#endif // LOGIN_H
