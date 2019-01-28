#ifndef SIGN_UP_H
#define SIGN_UP_H

#include <QDialog>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
    class Sign_up;
}

class Sign_up : public QDialog
{
    Q_OBJECT

public:
    explicit Sign_up(QWidget *parent = 0);
    ~Sign_up();

private:
    Ui::Sign_up *ui;
    QTcpSocket* socket;
    QTimer* timer;

private slots:
    void on_sign_up_clicked();
    void on_auth_code_clicked();
    void update();
    void socket_connect();
    void socket_error();
    void recv_data();

protected:
    //ÖØ»æÊÂ¼þ
    void paintEvent(QPaintEvent *event);
};

#endif // SIGN_UP_H
