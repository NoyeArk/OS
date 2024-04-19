#include "black_window.h"
#include "ui_black.h"

BlackWindow::BlackWindow(std::string code, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlackWindow)
{
    ui->setupUi(this);

    // 设置文本编辑器可滚动
    ui->outcome_text->setReadOnly(true);  // 设置为只读模式
    ui->outcome_text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // 始终显示垂直滚动条

    compiler.GCC(code);

    // 打开文本文件
    QFile file("D:\\qtonline\\run\\compiler\\output.txt");

    // 检查文件是否成功打开
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file";
    }

    QString fileContent;
    // 使用 QTextStream 读取文件内容
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        fileContent += line + "\n";
        qDebug() << line;
    }

    fileContent += "\n";
    fileContent += "\n";
    fileContent += "F:\\code\\os\\x64\\Debug\\os_file.exe (进程 23040)已退出，代码为 0。";
    fileContent += "\n";
    fileContent += "按任意键关闭此窗口. . .";
    ui->outcome_text->setPlainText(fileContent);
}

BlackWindow::~BlackWindow()
{
    delete ui;
}

void BlackWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // painter.drawPixmap(0,0,width(),height(),QPixmap("D:\\qtonline\\run\\i.PNG"));
}
