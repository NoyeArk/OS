/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLineEdit *cmdLineEdit;
    QLabel *label;
    QLabel *label_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QPushButton *pushButton_5;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(531, 419);
        cmdLineEdit = new QLineEdit(Widget);
        cmdLineEdit->setObjectName(QString::fromUtf8("cmdLineEdit"));
        cmdLineEdit->setGeometry(QRect(300, 270, 171, 21));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(160, 60, 261, 71));
        label->setStyleSheet(QString::fromUtf8("font: 28pt \"Agency FB\";"));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 240, 211, 81));
        label_2->setStyleSheet(QString::fromUtf8("font: 22pt \"Agency FB\";\n"
"font: 18pt \"Agency FB\";"));
        widget = new QWidget(Widget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 20, 221, 211));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMaximumSize(QSize(100, 100));
        pushButton->setStyleSheet(QString::fromUtf8("color: rgb(85, 0, 0);"));
        pushButton->setIconSize(QSize(100, 90));
        pushButton->setCheckable(false);

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(100, 100));
        pushButton_2->setIconSize(QSize(90, 100));

        verticalLayout->addWidget(pushButton_2);

        widget1 = new QWidget(Widget);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(210, 320, 260, 22));
        horizontalLayout = new QHBoxLayout(widget1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_4 = new QPushButton(widget1);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);

        pushButton_3 = new QPushButton(widget1);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_5 = new QPushButton(widget1);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        horizontalLayout->addWidget(pushButton_5);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\346\223\215\344\275\234\347\263\273\347\273\237", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        pushButton->setText(QString());
        pushButton_2->setText(QString());
        pushButton_4->setText(QCoreApplication::translate("Widget", "\347\241\256\345\256\232", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Widget", "\345\217\226\346\266\210", nullptr));
        pushButton_5->setText(QCoreApplication::translate("Widget", "\346\265\217\350\247\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
