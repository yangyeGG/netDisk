#ifndef LOGON_H
#define LOGON_H

#include <QDialog>

namespace Ui {
    class logon;
}

class logon : public QDialog
{
    Q_OBJECT

public:
    explicit logon(QWidget *parent = 0);
    ~logon();

private:
    Ui::logon *ui;

private slots:
    void on_pushButton_clicked();
};

#endif // LOGON_H
