/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Desktop
{
public:
    QLabel *label;
    QLabel *task_bar;
    QLabel *background;
    QPushButton *shutdown_btn;
    QPushButton *my_desktop;
    QPushButton *task_manager;

    void setupUi(QWidget *Desktop)
    {
        if (Desktop->objectName().isEmpty())
            Desktop->setObjectName(QString::fromUtf8("Desktop"));
        Desktop->resize(1024, 597);
        label = new QLabel(Desktop);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(510, 70, 331, 111));
        label->setStyleSheet(QString::fromUtf8("font: 28pt \"Agency FB\";"));
        task_bar = new QLabel(Desktop);
        task_bar->setObjectName(QString::fromUtf8("task_bar"));
        task_bar->setGeometry(QRect(0, 550, 1031, 51));
        task_bar->setStyleSheet(QString::fromUtf8("QLabel{background-color:rgb(255, 217, 101);}\n"
""));
        background = new QLabel(Desktop);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(-20, 0, 1111, 561));
        background->setStyleSheet(QString::fromUtf8("QLabel{background-color:rgb(197, 224, 179);}\n"
""));
        shutdown_btn = new QPushButton(Desktop);
        shutdown_btn->setObjectName(QString::fromUtf8("shutdown_btn"));
        shutdown_btn->setGeometry(QRect(20, 560, 31, 31));
        shutdown_btn->setMaximumSize(QSize(40, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8("F:/horiki/\346\223\215\344\275\234\347\263\273\347\273\237/\350\257\276\347\250\213\350\256\276\350\256\241/\346\234\252\346\240\207\351\242\230-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        shutdown_btn->setIcon(icon);
        shutdown_btn->setIconSize(QSize(31, 31));
        my_desktop = new QPushButton(Desktop);
        my_desktop->setObjectName(QString::fromUtf8("my_desktop"));
        my_desktop->setGeometry(QRect(20, 20, 80, 80));
        my_desktop->setMaximumSize(QSize(80, 80));
        my_desktop->setStyleSheet(QString::fromUtf8("color: rgb(85, 0, 0);"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("a.png"), QSize(), QIcon::Normal, QIcon::Off);
        my_desktop->setIcon(icon1);
        my_desktop->setIconSize(QSize(100, 90));
        my_desktop->setCheckable(false);
        task_manager = new QPushButton(Desktop);
        task_manager->setObjectName(QString::fromUtf8("task_manager"));
        task_manager->setGeometry(QRect(20, 110, 80, 80));
        task_manager->setMaximumSize(QSize(80, 80));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("b.png"), QSize(), QIcon::Normal, QIcon::Off);
        task_manager->setIcon(icon2);
        task_manager->setIconSize(QSize(90, 100));
        background->raise();
        label->raise();
        task_bar->raise();
        shutdown_btn->raise();
        my_desktop->raise();
        task_manager->raise();

        retranslateUi(Desktop);

        QMetaObject::connectSlotsByName(Desktop);
    } // setupUi

    void retranslateUi(QWidget *Desktop)
    {
        Desktop->setWindowTitle(QApplication::translate("Desktop", "Widget", nullptr));
        label->setText(QApplication::translate("Desktop", "Windows12 \346\246\202\345\277\265\347\211\210", nullptr));
        task_bar->setText(QString());
        background->setText(QString());
        shutdown_btn->setText(QString());
        my_desktop->setText(QString());
        task_manager->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Desktop: public Ui_Desktop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
