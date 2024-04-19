/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QPushButton *confirm_btn;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(1011, 597);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/win.png"), QSize(), QIcon::Normal, QIcon::Off);
        Login->setWindowIcon(icon);
        pushButton = new QPushButton(Login);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, -30, 1024, 768));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/login_background.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(16777215, 16777215));
        label = new QLabel(Login);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(420, 90, 201, 111));
        QFont font;
        font.setFamily(QString::fromUtf8("3ds"));
        font.setPointSize(48);
        label->setFont(font);
        label_2 = new QLabel(Login);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(410, 160, 201, 111));
        QFont font1;
        font1.setFamily(QString::fromUtf8("3ds"));
        font1.setPointSize(24);
        label_2->setFont(font1);
        lineEdit = new QLineEdit(Login);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(430, 300, 141, 31));
        lineEdit->setReadOnly(false);
        confirm_btn = new QPushButton(Login);
        confirm_btn->setObjectName(QString::fromUtf8("confirm_btn"));
        confirm_btn->setGeometry(QRect(460, 360, 71, 31));

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Windows12", nullptr));
        pushButton->setText(QString());
        label->setText(QApplication::translate("Login", "17:31", nullptr));
        label_2->setText(QApplication::translate("Login", "\346\230\237\346\234\237\344\272\224 1\346\234\2105\345\217\267", nullptr));
        lineEdit->setPlaceholderText(QApplication::translate("Login", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        confirm_btn->setText(QApplication::translate("Login", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
