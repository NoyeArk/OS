#ifndef DESKTOP_H
#define DESKTOP_H

#include <QWidget>
#include <QProcess>
#include <QMessageBox>
#include <folder.h>

#include "task_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Desktop; }
QT_END_NAMESPACE

class Desktop : public QWidget
{
    Q_OBJECT

public:
    Desktop(QWidget *parent = nullptr);
    ~Desktop();

private slots:
    void on_my_desktop_clicked();
    void on_task_manager_clicked();
    void on_shutdown_btn_clicked();

private:
    FCB* rootFcb;
    Ui::Desktop *ui;
};

#endif // DESKTOP_H
