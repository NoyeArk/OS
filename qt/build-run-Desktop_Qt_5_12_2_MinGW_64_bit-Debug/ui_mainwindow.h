/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_2;
    QAction *action_o;
    QAction *action_3;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_E;
    QMenu *menu_B;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(495, 364);
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_o = new QAction(MainWindow);
        action_o->setObjectName(QString::fromUtf8("action_o"));
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 495, 21));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_E = new QMenu(menubar);
        menu_E->setObjectName(QString::fromUtf8("menu_E"));
        menu_B = new QMenu(menubar);
        menu_B->setObjectName(QString::fromUtf8("menu_B"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_E->menuAction());
        menubar->addAction(menu_B->menuAction());
        menu->addSeparator();
        menu->addAction(action_2);
        menu->addAction(action_o);
        menu->addAction(action_3);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_2->setText(QApplication::translate("MainWindow", "\345\206\205\345\255\230\347\256\241\347\220\206(N)", nullptr));
        action_o->setText(QApplication::translate("MainWindow", "\345\244\226\345\255\230\347\256\241\347\220\206(O)", nullptr));
        action_3->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272(C)", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\344\273\273\345\212\241\347\256\241\347\220\206\345\231\250(F)", nullptr));
        menu_E->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\257\221(E)", nullptr));
        menu_B->setTitle(QApplication::translate("MainWindow", "\346\236\204\345\273\272(B)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
