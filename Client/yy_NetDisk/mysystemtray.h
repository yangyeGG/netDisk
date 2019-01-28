#ifndef MYSYSTEMTRAY_H
#define MYSYSTEMTRAY_H
#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>


class MySystemTray : public QWidget
{
    Q_OBJECT
public:
    explicit MySystemTray(QWidget *parent = nullptr);
    ~MySystemTray();
    void createActions();
    void createMenu();
signals:

private:
    QSystemTrayIcon* mSysTrayIcon;//œµÕ≥Õ–≈Ã¿‡
    QWidget* parent;
    QMenu *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
public slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_showMainAction();
    void on_exitAppAction();


};

#endif // MYSYSTEMTRAY_H
