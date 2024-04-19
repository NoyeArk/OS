#include "file1.h"
#include "ui_file1.h"

file1::file1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file1)
{
    ui->setupUi(this);
    //按钮上放图片
    QIcon con("D:\\qtonline\\run\\f.PNG");
    ui->pushButton->setIcon(con);

    QIcon cod("D:\\qtonline\\run\\g.PNG");
    ui->pushButton_2->setIcon(cod);
}

file1::~file1()
{
    delete ui;
}
