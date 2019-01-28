#ifndef WAITDIG_H
#define WAITDIG_H

#include <QDialog>

namespace Ui {
class WaitDig;
}

class WaitDig : public QDialog
{
    Q_OBJECT

public:
    explicit WaitDig(QWidget *parent = nullptr);
    ~WaitDig();

private:
    Ui::WaitDig *ui;
};

#endif // WAITDIG_H
