#include "input_dialog.h"

InputDialog::InputDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("文件重命名");

    inputLineEdit = new QLineEdit(this);

    okButton = new QPushButton("确定", this);
    cancelButton = new QPushButton("取消", this);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(inputLineEdit);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    setLayout(layout);
}

QString InputDialog::getText() const {
    return inputLineEdit->text();
}
