#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>

#include "desktop.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void CheckPassword();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
