/********************************************************************************
** Form generated from reading UI file 'property.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Property
{
public:
    QLabel *type_label;
    QLabel *size_label;
    QLabel *location_label;
    QLabel *property_label;
    QLabel *creat_time_label;
    QLabel *update_time_label;
    QLabel *type_content_label;
    QLabel *size_content_label;
    QLabel *location_content_label;
    QLabel *property_content_label;
    QLabel *creat_time_content_label;
    QLabel *update_time_content_label;

    void setupUi(QWidget *Property)
    {
        if (Property->objectName().isEmpty())
            Property->setObjectName(QString::fromUtf8("Property"));
        Property->resize(227, 270);
        type_label = new QLabel(Property);
        type_label->setObjectName(QString::fromUtf8("type_label"));
        type_label->setGeometry(QRect(40, 20, 54, 31));
        size_label = new QLabel(Property);
        size_label->setObjectName(QString::fromUtf8("size_label"));
        size_label->setGeometry(QRect(40, 60, 54, 31));
        location_label = new QLabel(Property);
        location_label->setObjectName(QString::fromUtf8("location_label"));
        location_label->setGeometry(QRect(40, 100, 54, 31));
        property_label = new QLabel(Property);
        property_label->setObjectName(QString::fromUtf8("property_label"));
        property_label->setGeometry(QRect(40, 140, 54, 31));
        creat_time_label = new QLabel(Property);
        creat_time_label->setObjectName(QString::fromUtf8("creat_time_label"));
        creat_time_label->setGeometry(QRect(40, 170, 54, 31));
        update_time_label = new QLabel(Property);
        update_time_label->setObjectName(QString::fromUtf8("update_time_label"));
        update_time_label->setGeometry(QRect(40, 210, 54, 31));
        type_content_label = new QLabel(Property);
        type_content_label->setObjectName(QString::fromUtf8("type_content_label"));
        type_content_label->setGeometry(QRect(120, 30, 54, 12));
        size_content_label = new QLabel(Property);
        size_content_label->setObjectName(QString::fromUtf8("size_content_label"));
        size_content_label->setGeometry(QRect(120, 70, 54, 12));
        location_content_label = new QLabel(Property);
        location_content_label->setObjectName(QString::fromUtf8("location_content_label"));
        location_content_label->setGeometry(QRect(120, 110, 54, 12));
        property_content_label = new QLabel(Property);
        property_content_label->setObjectName(QString::fromUtf8("property_content_label"));
        property_content_label->setGeometry(QRect(120, 150, 54, 12));
        creat_time_content_label = new QLabel(Property);
        creat_time_content_label->setObjectName(QString::fromUtf8("creat_time_content_label"));
        creat_time_content_label->setGeometry(QRect(120, 180, 54, 12));
        update_time_content_label = new QLabel(Property);
        update_time_content_label->setObjectName(QString::fromUtf8("update_time_content_label"));
        update_time_content_label->setGeometry(QRect(120, 220, 54, 12));

        retranslateUi(Property);

        QMetaObject::connectSlotsByName(Property);
    } // setupUi

    void retranslateUi(QWidget *Property)
    {
        Property->setWindowTitle(QApplication::translate("Property", "\346\226\207\344\273\266\345\261\236\346\200\247", nullptr));
        type_label->setText(QApplication::translate("Property", "\346\226\207\344\273\266\347\261\273\345\236\213", nullptr));
        size_label->setText(QApplication::translate("Property", "\346\226\207\344\273\266\345\244\247\345\260\217", nullptr));
        location_label->setText(QApplication::translate("Property", "\344\275\215\347\275\256", nullptr));
        property_label->setText(QApplication::translate("Property", "\345\261\236\346\200\247", nullptr));
        creat_time_label->setText(QApplication::translate("Property", "\345\210\233\345\273\272\346\227\266\351\227\264", nullptr));
        update_time_label->setText(QApplication::translate("Property", "\344\277\256\346\224\271\346\227\266\351\227\264", nullptr));
        type_content_label->setText(QApplication::translate("Property", "TextLabel", nullptr));
        size_content_label->setText(QApplication::translate("Property", "TextLabel", nullptr));
        location_content_label->setText(QApplication::translate("Property", "TextLabel", nullptr));
        property_content_label->setText(QApplication::translate("Property", "TextLabel", nullptr));
        creat_time_content_label->setText(QApplication::translate("Property", "TextLabel", nullptr));
        update_time_content_label->setText(QApplication::translate("Property", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Property: public Ui_Property {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTY_H
