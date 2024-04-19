#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEdit->setEchoMode(QLineEdit::Password);  // 设置为密码输入模式
    connect(ui->confirm_btn, &QPushButton::clicked, this, &Login::CheckPassword);
}

Login::~Login()
{
    delete ui;
}

void Login::CheckPassword()
{
    // qDebug() << "correct!";
    // 获取 QLineEdit 中的文本内容
    QString enteredText = ui->lineEdit->text();

    // 检查文本是否为空
    if (enteredText.isEmpty()) {
        // 文本为空的处理逻辑，例如显示错误消息
        QMessageBox::warning(this, "Error", "Please enter some text.");
    } else {
        if (enteredText == "neuq") {
            this->hide();
            Desktop *desktop = new Desktop();
            desktop->show();
        }
        else {
            QMessageBox::warning(this, "Error", "密码错误");
            ui->lineEdit->clear();
        }
    }
}
