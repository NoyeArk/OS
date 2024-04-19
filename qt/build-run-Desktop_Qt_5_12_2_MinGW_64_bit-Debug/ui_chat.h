/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Folder
{
public:
    QLabel *label;
    QPushButton *back_btn;
    QLabel *label_2;
    QLabel *file_path_label;

    void setupUi(QWidget *Folder)
    {
        if (Folder->objectName().isEmpty())
            Folder->setObjectName(QString::fromUtf8("Folder"));
        Folder->resize(799, 528);
        label = new QLabel(Folder);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 811, 41));
        label->setStyleSheet(QString::fromUtf8("QLabel{background-color:rgb(70, 114, 196);}\n"
""));
        back_btn = new QPushButton(Folder);
        back_btn->setObjectName(QString::fromUtf8("back_btn"));
        back_btn->setGeometry(QRect(10, 10, 41, 21));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QQ\345\233\276\347\211\20720240103153831.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        back_btn->setIcon(icon);
        back_btn->setIconSize(QSize(50, 50));
        label_2 = new QLabel(Folder);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 40, 801, 491));
        label_2->setStyleSheet(QString::fromUtf8("QLabel{background-color:rgb(255, 255, 255);}\n"
""));
        file_path_label = new QLabel(Folder);
        file_path_label->setObjectName(QString::fromUtf8("file_path_label"));
        file_path_label->setGeometry(QRect(80, 10, 371, 20));
        file_path_label->setStyleSheet(QString::fromUtf8("font: 11pt \"Consolas\";"));
        label_2->raise();
        label->raise();
        back_btn->raise();
        file_path_label->raise();

        retranslateUi(Folder);

        QMetaObject::connectSlotsByName(Folder);
    } // setupUi

    void retranslateUi(QWidget *Folder)
    {
        Folder->setWindowTitle(QApplication::translate("Folder", "\346\226\207\344\273\266\350\265\204\346\272\220\347\256\241\347\220\206\345\231\250", nullptr));
        label->setText(QString());
        back_btn->setText(QString());
        label_2->setText(QString());
        file_path_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Folder: public Ui_Folder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
