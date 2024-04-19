/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chat
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QString::fromUtf8("Chat"));
        Chat->resize(474, 335);
        widget = new QWidget(Chat);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(110, 10, 254, 251));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_1 = new QPushButton(widget);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setMinimumSize(QSize(50, 50));
        pushButton_1->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButton_1);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(50, 50));
        pushButton_2->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(50, 50));
        pushButton_3->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButton_3);


        retranslateUi(Chat);

        QMetaObject::connectSlotsByName(Chat);
    } // setupUi

    void retranslateUi(QWidget *Chat)
    {
        Chat->setWindowTitle(QCoreApplication::translate("Chat", "Form", nullptr));
        pushButton_1->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
