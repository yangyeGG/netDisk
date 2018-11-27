#ifndef XMB_H
#define XMB_H

#include <QDialog>

namespace Ui {
    class XMB;
}

class XMB : public QDialog
{
    Q_OBJECT

public:
    explicit XMB(QWidget *parent = 0);
    ~XMB();

private:
    Ui::XMB *ui;
};

#endif // XMB_H
