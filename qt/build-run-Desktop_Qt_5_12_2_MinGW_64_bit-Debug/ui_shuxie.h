/********************************************************************************
** Form generated from reading UI file 'shuxie.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHUXIE_H
#define UI_SHUXIE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_shuxie
{
public:
    QAction *action_N;
    QAction *action_N_2;
    QAction *save_action;
    QWidget *centralwidget;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_S;
    QMenu *menu_L;
    QMenu *menu_E;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *shuxie)
    {
        if (shuxie->objectName().isEmpty())
            shuxie->setObjectName(QString::fromUtf8("shuxie"));
        shuxie->resize(554, 459);
        action_N = new QAction(shuxie);
        action_N->setObjectName(QString::fromUtf8("action_N"));
        action_N_2 = new QAction(shuxie);
        action_N_2->setObjectName(QString::fromUtf8("action_N_2"));
        save_action = new QAction(shuxie);
        save_action->setObjectName(QString::fromUtf8("save_action"));
        centralwidget = new QWidget(shuxie);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(0, 0, 561, 421));
        textEdit->setStyleSheet(QString::fromUtf8("font: 12pt \"Consolas\";"));
        shuxie->setCentralWidget(centralwidget);
        menubar = new QMenuBar(shuxie);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 554, 23));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_S = new QMenu(menubar);
        menu_S->setObjectName(QString::fromUtf8("menu_S"));
        menu_L = new QMenu(menubar);
        menu_L->setObjectName(QString::fromUtf8("menu_L"));
        menu_E = new QMenu(menubar);
        menu_E->setObjectName(QString::fromUtf8("menu_E"));
        shuxie->setMenuBar(menubar);
        statusbar = new QStatusBar(shuxie);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        shuxie->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_S->menuAction());
        menubar->addAction(menu_L->menuAction());
        menubar->addAction(menu_E->menuAction());
        menu->addAction(action_N);
        menu->addAction(action_N_2);
        menu->addAction(save_action);

        retranslateUi(shuxie);

        QMetaObject::connectSlotsByName(shuxie);
    } // setupUi

    void retranslateUi(QMainWindow *shuxie)
    {
        shuxie->setWindowTitle(QApplication::translate("shuxie", "\346\226\207\346\234\254\347\274\226\350\276\221\345\231\250", nullptr));
        action_N->setText(QApplication::translate("shuxie", "\346\226\260\345\273\272\357\274\210N\357\274\211", nullptr));
        action_N_2->setText(QApplication::translate("shuxie", "\347\274\226\350\276\221\357\274\210N\357\274\211", nullptr));
        save_action->setText(QApplication::translate("shuxie", "\344\277\235\345\255\230\357\274\210A\357\274\211", nullptr));
        menu->setTitle(QApplication::translate("shuxie", "\346\226\207\344\273\266\357\274\210F\357\274\211", nullptr));
        menu_S->setTitle(QApplication::translate("shuxie", "\345\217\246\345\255\230\344\270\272\357\274\210S\357\274\211", nullptr));
        menu_L->setTitle(QApplication::translate("shuxie", "\346\237\245\347\234\213\357\274\210L\357\274\211", nullptr));
        menu_E->setTitle(QApplication::translate("shuxie", "\351\200\200\345\207\272\357\274\210E\357\274\211", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditTxt: public Ui_shuxie {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHUXIE_H
