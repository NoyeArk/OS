/********************************************************************************
** Form generated from reading UI file 'txt_edit.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TXT_EDIT_H
#define UI_TXT_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TxtEdit
{
public:
    QLineEdit *lineEdit;

    void setupUi(QWidget *TxtEdit)
    {
        if (TxtEdit->objectName().isEmpty())
            TxtEdit->setObjectName(QString::fromUtf8("TxtEdit"));
        TxtEdit->resize(470, 367);
        lineEdit = new QLineEdit(TxtEdit);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(10, 50, 441, 311));

        retranslateUi(TxtEdit);

        QMetaObject::connectSlotsByName(TxtEdit);
    } // setupUi

    void retranslateUi(QWidget *TxtEdit)
    {
        TxtEdit->setWindowTitle(QApplication::translate("TxtEdit", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TxtEdit: public Ui_TxtEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TXT_EDIT_H
