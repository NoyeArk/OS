/********************************************************************************
** Form generated from reading UI file 'menu.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_H
#define UI_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Menu
{
public:
    QPushButton *update_btn;
    QPushButton *property_btn;
    QPushButton *compiler_btn;
    QPushButton *new_folder_btn;
    QPushButton *new_txt_btn;
    QPushButton *rename_btn;
    QLabel *label_2;
    QPushButton *delete_btn;

    void setupUi(QWidget *Menu)
    {
        if (Menu->objectName().isEmpty())
            Menu->setObjectName(QString::fromUtf8("Menu"));
        Menu->resize(111, 210);
        update_btn = new QPushButton(Menu);
        update_btn->setObjectName(QString::fromUtf8("update_btn"));
        update_btn->setGeometry(QRect(0, 0, 111, 31));
        update_btn->setStyleSheet(QString::fromUtf8("font: 9pt \"3ds\";\n"
"font: 14pt \"3ds\";"));
        property_btn = new QPushButton(Menu);
        property_btn->setObjectName(QString::fromUtf8("property_btn"));
        property_btn->setGeometry(QRect(0, 30, 111, 31));
        property_btn->setStyleSheet(QString::fromUtf8("font: 9pt \"3ds\";\n"
"font: 14pt \"3ds\";"));
        compiler_btn = new QPushButton(Menu);
        compiler_btn->setObjectName(QString::fromUtf8("compiler_btn"));
        compiler_btn->setGeometry(QRect(0, 60, 111, 31));
        compiler_btn->setStyleSheet(QString::fromUtf8("font: 9pt \"3ds\";\n"
"font: 14pt \"3ds\";"));
        new_folder_btn = new QPushButton(Menu);
        new_folder_btn->setObjectName(QString::fromUtf8("new_folder_btn"));
        new_folder_btn->setGeometry(QRect(0, 90, 111, 31));
        new_folder_btn->setStyleSheet(QString::fromUtf8("font: 9pt \"3ds\";\n"
"font: 14pt \"3ds\";"));
        new_txt_btn = new QPushButton(Menu);
        new_txt_btn->setObjectName(QString::fromUtf8("new_txt_btn"));
        new_txt_btn->setGeometry(QRect(0, 120, 111, 31));
        new_txt_btn->setStyleSheet(QString::fromUtf8("font: 9pt \"3ds\";\n"
"font: 14pt \"3ds\";"));
        rename_btn = new QPushButton(Menu);
        rename_btn->setObjectName(QString::fromUtf8("rename_btn"));
        rename_btn->setGeometry(QRect(0, 180, 111, 31));
        rename_btn->setStyleSheet(QString::fromUtf8("font: 9pt \"3ds\";\n"
"font: 14pt \"3ds\";"));
        label_2 = new QLabel(Menu);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 1, 111, 211));
        label_2->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);\n"
"border: 2px;"));
        delete_btn = new QPushButton(Menu);
        delete_btn->setObjectName(QString::fromUtf8("delete_btn"));
        delete_btn->setGeometry(QRect(0, 150, 111, 31));
        delete_btn->setStyleSheet(QString::fromUtf8("font: 9pt \"3ds\";\n"
"font: 14pt \"3ds\";"));
        label_2->raise();
        update_btn->raise();
        property_btn->raise();
        compiler_btn->raise();
        new_folder_btn->raise();
        new_txt_btn->raise();
        rename_btn->raise();
        delete_btn->raise();

        retranslateUi(Menu);

        QMetaObject::connectSlotsByName(Menu);
    } // setupUi

    void retranslateUi(QWidget *Menu)
    {
        Menu->setWindowTitle(QApplication::translate("Menu", "Form", nullptr));
        update_btn->setText(QApplication::translate("Menu", "\345\210\267\346\226\260", nullptr));
        property_btn->setText(QApplication::translate("Menu", "\345\261\236\346\200\247", nullptr));
        compiler_btn->setText(QApplication::translate("Menu", "\347\274\226\350\257\221\346\211\247\350\241\214", nullptr));
        new_folder_btn->setText(QApplication::translate("Menu", "\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        new_txt_btn->setText(QApplication::translate("Menu", "\346\226\260\345\273\272\346\226\207\344\273\266", nullptr));
        rename_btn->setText(QApplication::translate("Menu", "\351\207\215\345\221\275\345\220\215", nullptr));
        label_2->setText(QApplication::translate("Menu", "TextLabel", nullptr));
        delete_btn->setText(QApplication::translate("Menu", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Menu: public Ui_Menu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_H
