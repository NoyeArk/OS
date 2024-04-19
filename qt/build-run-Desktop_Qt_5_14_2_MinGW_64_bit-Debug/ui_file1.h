/********************************************************************************
** Form generated from reading UI file 'file1.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE1_H
#define UI_FILE1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_file1
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *file1)
    {
        if (file1->objectName().isEmpty())
            file1->setObjectName(QString::fromUtf8("file1"));
        file1->resize(400, 300);
        widget = new QWidget(file1);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(110, 30, 168, 81));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(50, 50));
        pushButton->setIconSize(QSize(70, 70));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(50, 50));
        pushButton_2->setIconSize(QSize(70, 70));

        horizontalLayout->addWidget(pushButton_2);


        retranslateUi(file1);

        QMetaObject::connectSlotsByName(file1);
    } // setupUi

    void retranslateUi(QWidget *file1)
    {
        file1->setWindowTitle(QCoreApplication::translate("file1", "Form", nullptr));
        pushButton->setText(QString());
        pushButton_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class file1: public Ui_file1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE1_H
