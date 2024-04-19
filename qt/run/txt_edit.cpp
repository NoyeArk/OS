#include "txt_edit.h"
#include "ui_shuxie.h"

EditTxt::EditTxt(FCB* parent_fcb, std::vector<std::string>& data, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditTxt)
{
    ui->setupUi(this);

    myFcb = parent_fcb;

    for (auto const& blockData : data) {
        fileContentToDisplay += blockData;
        qDebug() << "显示的内容为" << QString::fromStdString(fileContentToDisplay);
    }

    // 设置文本内容
    ui->textEdit->setPlainText(QString::fromStdString(fileContentToDisplay));
    qDebug() << "显示成功";

    QObject::connect(ui->save_action, &QAction::triggered, this, &EditTxt::save_txt);

}


EditTxt::~EditTxt()
{
    delete ui;
}


void EditTxt::save_txt()
{
    // 获取文本内容
    QString content = ui->textEdit->toPlainText();
    bool isWriteOk = kernel.SysWriteFile(myFcb, currentFileName, content.toStdString());
    if (isWriteOk) {
        QMessageBox::information(this, "Success", "保存成功！");
    }
    else {
        QMessageBox::warning(this, "Error", "保存失败！");
    }
    this->close();
}
