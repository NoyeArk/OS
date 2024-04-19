#include "folder.h"
#include "ui_chat.h"


Folder::Folder(FCB* fcb, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Folder)
{
    ui->setupUi(this);
    next_x = 40;
    next_y = 50;
    folder_cnt = 0;

    num = 0;
    time_Start = 0 ;
    time_End = 0 ;
    myFcb = fcb;

    init_folder();

    menu = new Menu();
    qDebug() << "构造函数::当前目录的路径是" << QString::fromStdString(myFcb->path);

    timer.setInterval(500);
    connect(&timer, &QTimer::timeout, this, &Folder::judge_menu_inst);
    timer.start();   // 启动定时器

    // 创建定时器
    update_timer = new QTimer(this);
    connect(update_timer, SIGNAL(timeout()), this, SLOT(update_window()));

    menu->setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->back_btn, &QPushButton::clicked, this, &Folder::on_back_btn_clicked);
    ui->file_path_label->setText(myFcb->path.c_str());
}


Folder::~Folder()
{
    delete ui;
}


void Folder::on_pushButton_1_clicked()
{
    this->hide();
    file1 *c = new file1();
    c->show();
}

void Folder::on_back_btn_clicked()
{
    this->hide();
}

void Folder::update_file_path()
{
    std::string curFilePath = kernel.getCurPath();
    ui->file_path_label->setText(curFilePath.c_str());
}

void Folder::init_folder()
{
    for (size_t ii = 0; ii < myFcb->childFiles.size(); ii++) {
        qDebug() << "子文件名：" << QString::fromStdString(myFcb->childFiles[ii]->name);
    }
    // 判断当前文件中有多少个子文件，把他们画出来
    for (size_t ii = 0; ii < myFcb->childFiles.size(); ii++) {
        if (myFcb->childFiles[ii]->type == DIR) {
            qDebug() << "绘制文件" << QString::fromStdString(myFcb->childFiles[ii]->name);
            new_folder(false, myFcb->childFiles[ii]->name);
        }
        else if (myFcb->childFiles[ii]->type == DATA) {
            new_txt(false, myFcb->childFiles[ii]->name);
        }
    }
}

FCB *Folder::open_file(std::string fileName)
{
    qDebug() << "open_file::当前文件：" << QString::fromStdString(fileName);
    qDebug() << "open_file::当前路径：" << QString::fromStdString(myFcb->path);
    for (size_t ii = 0; ii < myFcb->childFiles.size(); ii++) {
        if (myFcb->childFiles[ii]->name == fileName) {
            myFcb->childFiles[ii]->authorization = emDelDenied;
            return myFcb->childFiles[ii];
        }
    }
    return nullptr;
}

void Folder::update_window()
{
    QColor color = palette().color(QPalette::Button);
    int lightness = color.lightness();
    if (lightness > 95)
        direction = -1;
    else if (lightness < 80)
        direction = 1;
    lightness += 5 * direction;

    QPalette pal = palette();
    pal.setColor(QPalette::Button, QColor::fromHsl(color.hue(), color.saturation(), lightness));
    setPalette(pal);
}

void Folder::judge_double_click()
{
    num += 1;
    if(num == 1){
        time_Start = (double)clock();

        if((time_Start - time_End) < 500){  // 连续快速两次单击

        }
    }

    if(num == 2){
        num = 0;
        time_End = (double)clock();
        if((time_End - time_Start) < 500){  // 连续快速两次单击
            qDebug()<< "-------------双击打开文件--------------";
            qDebug() << "judge_double_click::当前目录为" << QString::fromStdString(curFileName);
            FCB* fcb = open_file(curFileName);
            if (fcb == nullptr) {
                qDebug() << "打开目标文件为空指针";
                qDebug() << "11111";
                return;
            }
            qDebug()<< "打开文件" << QString::fromStdString(curFileName);;
            if (fcb->type == DIR) {
                open_folder();
            }
            else if (fcb->type == DATA) {
                open_txt_editor();
            }

            qDebug()<< "click quickly " << time_End - time_Start ;
        }
    }
}


void Folder::display_property()
{
    FCB* fcb = open_file(curFileName);

    if (fcb == nullptr) {
        qDebug() << "属性为空";
        return ;
    }

    auto createTime = fcb->createTime;
    std::time_t createTime_t = std::chrono::system_clock::to_time_t(createTime);

    // 使用 localtime_s 替代 localtime
    std::tm localTime;
    localtime_s(&localTime, &createTime_t);

    // 使用 std::ostringstream 构建字符串
    std::ostringstream createTimeString;
    createTimeString << (localTime.tm_hour % 12 == 0 ? 12 : localTime.tm_hour % 12) << ":"
        << (localTime.tm_min < 10 ? "0" : "") << localTime.tm_min
        << " " << (localTime.tm_hour >= 12 ? "PM" : "AM");

    auto updateTime = fcb->lastEditTime;
    std::time_t updateTime_t = std::chrono::system_clock::to_time_t(updateTime);

    std::tm localTime1;
    localtime_s(&localTime1, &updateTime_t);

    // 使用 std::ostringstream 构建字符串
    std::ostringstream updateTimeString;
    updateTimeString << (localTime.tm_hour % 12 == 0 ? 12 : localTime.tm_hour % 12) << ":"
        << (localTime.tm_min < 10 ? "0" : "") << localTime.tm_min
        << " " << (localTime.tm_hour >= 12 ? "PM" : "AM");

    std::string property = (fcb->authorization == 1) ? "可写可读" : "只读";
    std::string type = (fcb->type == DIR) ? "目录文件" : "数据文件";;

    Property *cur = new Property();
    cur->set_size(std::to_string(fcb->len));
    cur->set_type(type);
    cur->set_location("C盘");
    cur->set_property(property);
    cur->set_creat_time(createTimeString.str());
    cur->set_update_time(updateTimeString.str());

    cur->show();
}


void Folder::judge_menu_inst()
{
    if (menu->is_new_folder == true) {
        new_folder(true);
        menu->is_new_folder = false;
        menu->hide();
    }
    else if (menu->is_rename == true) {
        menu->hide();
        rename();
        menu->is_rename = false;
    }
    else if (menu->is_property == true) {
        display_property();
        menu->is_property = false;
        menu->hide();
    }
    else if (menu->is_new_txt == true) {
        menu->hide();
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor)); //等待旋转
        menu->is_new_txt = false;
        new_txt(true);
        QApplication::restoreOverrideCursor(); //恢复
    }
    else if (menu->is_compiler == true) {
        compiler();
        menu->is_compiler = false;
        menu->hide();
    }
    else if (menu->is_update == true) {
        menu->hide();
        update_timer->start(50);
        menu->is_update = false;
    }
    else if (menu->is_delete == true) {
        menu->hide();
        menu->is_delete = false;
        delete_file();
    }
}


void Folder::new_folder(bool isNewFolder, std::string fileName)
{
    std::string folderName;
    if (isNewFolder)
        folderName = "新建文件夹" + std::to_string(++folder_cnt);
    else
        folderName = fileName;

    for (size_t ii = 0; ii < folders.size(); ii++) {
        if (folders[ii].isVisible == false) {
            folders[ii].isVisible = true;
            folders[ii].newButton->setVisible(true);
            folders[ii].newLabel->setVisible(true);
            folders[ii].origin_name = folderName;
            folders[ii].my_name = folders[ii].origin_name;
            return;
        }
    }

    // 创建新的按钮
    FolerSet newFolder;
    newFolder.isVisible = true;
    newFolder.origin_name = folderName;
    newFolder.my_name = newFolder.origin_name;
    newFolder.newButton = new QPushButton("", this);
    newFolder.newButton->setObjectName(QString::fromStdString(folderName));  // 设置按钮的对象名称
    QIcon cog("D:\\qtonline\\run\\h.PNG");
    newFolder.newButton->setIcon(cog);
    newFolder.newButton->setIconSize(QSize(50, 50));
    newFolder.newButton->setGeometry(next_x, next_y, 65, 65);

    qDebug() << "next_x " << QString::number(next_x);
    qDebug() << "next_y " << QString::number(next_y);

    // 连接新按钮的点击信号到槽函数
    connect(newFolder.newButton, &QPushButton::clicked, [=]() {
        update_current_file_name(folderName);
    });

    // 创建新的标签用于显示文件名称
    newFolder.newLabel = new QLabel(this);

    newFolder.newLabel->setText(QString::fromStdString(folderName));
    newFolder.newLabel->move(next_x, next_y + 70);

    newFolder.newLabel->show();
    newFolder.newButton->show();

    folders.push_back(newFolder);

    next_x += 80;
    if (next_x>=455) {
        next_y += 80;
        next_x = 0;
    }

    kernel.directory.mkdir(myFcb, folderName);
}


void Folder::new_txt(bool isNewFolder, std::string fileName)
{
    std::string txtName;
    if (isNewFolder)
        txtName = "新建文件" + std::to_string(++folder_cnt);
    else
        txtName = fileName;

    for (size_t ii = 0; ii < folders.size(); ii++) {
        if (folders[ii].isVisible == false) {
            folders[ii].isVisible = true;
            folders[ii].newButton->setVisible(true);
            folders[ii].newLabel->setVisible(true);
            folders[ii].origin_name = txtName;
            folders[ii].my_name = folders[ii].origin_name;
            return;
        }
    }

    // 创建新的按钮
    FolerSet newFolder;
    newFolder.isVisible = true;
    newFolder.origin_name = txtName;
    newFolder.my_name = newFolder.origin_name;
    newFolder.newButton = new QPushButton("", this);
    newFolder.newButton->setObjectName(txtName.c_str());  // 设置按钮的对象名称
    QIcon cog("D:\\qtonline\\run\\txt.PNG");
    newFolder.newButton->setIcon(cog);
    newFolder.newButton->setIconSize(QSize(50, 50));
    newFolder.newButton->setGeometry(next_x, next_y, 65, 65);

    // 连接新按钮的点击信号到槽函数
    connect(newFolder.newButton, &QPushButton::clicked, [=]() {
        update_current_file_name(txtName);
    });

    // 创建新的标签用于显示文件名称
    newFolder.newLabel = new QLabel(this);

    newFolder.newLabel->setText(txtName.c_str());
    newFolder.newLabel->move(next_x, next_y + 70);

    newFolder.newLabel->show();
    newFolder.newButton->show();

    folders.push_back(newFolder);

    next_x += 80;
    if (next_x>=455) {
        next_y += 80;
        next_x = 0;
    }

    // 逻辑上创建文件
    kernel.SysCreateFile(myFcb, txtName);
}


void Folder::mousePressEvent(QMouseEvent *event)
{
    qDebug() << event->x() << ":" << event->y();
    for (size_t ii = 0; ii < myFcb->childFiles.size(); ii++) {
        qDebug() << "子文件名：" << QString::fromStdString(myFcb->childFiles[ii]->name);
    }
    if(event->button() == Qt::LeftButton) {
        menu->hide();
        judge_double_click();
    }

    else if(event->button() == Qt::RightButton) {
        QPoint globalPos = mapToGlobal(event->pos());  // 获取鼠标事件的全局坐标
        menu->move(globalPos);  // 将菜单移动到全局坐标位置
        menu->show();
    }
}


void Folder::mouseDoubleClickEvent(QMouseEvent *event)
{

}


void Folder::open_folder()
{
//    kernel.directory.cd(curFileName);
    auto fcb = open_file(curFileName);
    fcb->UpdateFilePath();
    Folder *cur = new Folder(fcb);
    cur->show();
}

void Folder::delete_file()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Confirmation", "确定要删除该文件吗?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool isDeleteOk = kernel.SysDeleteFile(myFcb, curFileName);
        if (!isDeleteOk) {
            // 删除失败
            qDebug() << "删除失败";
            for (size_t ii = 0; ii < myFcb->childFiles.size(); ii++) {
                if (myFcb->childFiles[ii]->name != curFileName)
                    continue;
                if (myFcb->childFiles[ii]->type == DIR) {
                    QMessageBox::warning(this, "Error", "当前文件非空，不允许删除！");
                }
                else if (myFcb->childFiles[ii]->type == DIR) {
                    QMessageBox::warning(this, "Error", "当前文件在内存中，不允许删除！");
                }
            }
        }
        else {
            // 删除成功
            qDebug() << "开始删除图标";
            for (size_t ii = 0; ii < folders.size(); ii++) {
                qDebug() << "要删除的文件是:" << QString::fromStdString(curFileName);
                qDebug() << "已有文件是:" << QString::fromStdString(folders[ii].my_name);
                if (folders[ii].my_name == curFileName) {
                    qDebug() << "删除图标";
                    folders[ii].isVisible = false;
                    folders[ii].newButton->setVisible(false);
                    folders[ii].newLabel->setVisible(false);
                    break;
                }
            }
        }
    }
}


void Folder::open_txt_editor()
{
   std::vector<std::string> data;
    kernel.SysOpenFile(myFcb, curFileName);

    auto fcb = open_file(curFileName);
    // fcb->UpdateFilePath();
    // 读取文件内容
    bool isReadOk = kernel.SysReadFile(curFileName, data);

    if (!isReadOk) {
        qDebug() << "读取失败";
    }

    // myFcb = kernel.SysOpenFile(curFileName);
    EditTxt *cur = new EditTxt(fcb, data);

    cur->show();
}


void Folder::rename()
{
    QString targetFileName;
    InputDialog *inputDialog = new InputDialog();
    if (inputDialog->exec() == QDialog::Accepted) {
        targetFileName = inputDialog->getText();
        qDebug() << "User Input: " << targetFileName;
    }
    kernel.directory.Rename(myFcb, curFileName, targetFileName.toStdString());

    // 更新界面文件名称
    for (size_t ii = 0; ii < folders.size(); ii++) {
        if (folders[ii].origin_name == curFileName) {
            folders[ii].my_name = targetFileName.toStdString();
            folders[ii].newLabel->setText(targetFileName);
            break;
        }
    }
}


void Folder::compiler()
{
    std::vector<std::string> data;
    std::string code;
    kernel.SysOpenFile(myFcb, curFileName);
    auto fcb = open_file(curFileName);
    // fcb->UpdateFilePath();
    // 读取文件内容
    bool isReadOk = kernel.SysReadFile(curFileName, data);
    for (auto const& blockData : data) {
        code += blockData;
    }
    BlackWindow *cur = new BlackWindow(code);
    cur->show();
}


void Folder::update_current_file_name(const std::string& fileName)
{
    for (size_t ii = 0; ii < folders.size(); ii++) {
        if (folders[ii].origin_name == fileName) {
            curFileName = folders[ii].my_name;
            break;
        }
    }
    auto blocksId = kernel.disk.GetFreeBlockId();
    for (auto id : blocksId) {
        qDebug() << "当前空闲块为" << QString::number(id);
    }
    qDebug() << "更新当前文件为" << QString::fromStdString(curFileName);
    judge_double_click();
}
