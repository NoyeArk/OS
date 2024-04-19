/********************************************************************************
** Form generated from reading UI file 'black.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLACK_H
#define UI_BLACK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BlackWindow
{
public:
    QTextEdit *outcome_text;

    void setupUi(QWidget *BlackWindow)
    {
        if (BlackWindow->objectName().isEmpty())
            BlackWindow->setObjectName(QString::fromUtf8("BlackWindow"));
        BlackWindow->resize(681, 440);
        BlackWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        outcome_text = new QTextEdit(BlackWindow);
        outcome_text->setObjectName(QString::fromUtf8("outcome_text"));
        outcome_text->setGeometry(QRect(0, 0, 681, 441));
        outcome_text->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 13pt \"Consolas\";"));

        retranslateUi(BlackWindow);

        QMetaObject::connectSlotsByName(BlackWindow);
    } // setupUi

    void retranslateUi(QWidget *BlackWindow)
    {
        BlackWindow->setWindowTitle(QApplication::translate("BlackWindow", "\350\260\203\350\257\225\346\216\247\345\210\266\345\217\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BlackWindow: public Ui_BlackWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLACK_H
