#include "desktop.h"
#include "ui_widget.h"

Desktop::Desktop(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Desktop)
{
    ui->setupUi(this);

    rootFcb = new FCB();
    rootFcb->name = "C:";
    rootFcb->path = "C:";

    connect(ui->my_desktop, &QPushButton::clicked, this, &Desktop::on_my_desktop_clicked);
    connect(ui->task_manager, &QPushButton::clicked, this, &Desktop::on_task_manager_clicked);
    connect(ui->shutdown_btn, &QPushButton::clicked, this, &Desktop::on_shutdown_btn_clicked);

    //按钮上放图片
    QIcon con("D:\\qtonline\\run\\a.PNG");
    ui->my_desktop->setIcon(con);

    QIcon cod("D:\\qtonline\\run\\b.PNG");
    ui->task_manager->setIcon(cod);
}

Desktop::~Desktop()
{
    delete ui;
}


void Desktop::on_my_desktop_clicked()
{

    // auto fcb = kernel.directory.GetRootFcb();
    Folder *cur = new Folder(rootFcb);
    cur->show();
}

void Desktop::on_task_manager_clicked()
{
    MainWindow *c = new MainWindow();
    c->show();
}

void Desktop::on_shutdown_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Shutdown Confirmation", "确定要关机吗?",
                                  QMessageBox::Yes | QMessageBox::No);

    // 处理用户的选择
    if (reply == QMessageBox::Yes) {
        // 如果用户选择"Yes"，执行关机操作
        this->close();
    }
}
