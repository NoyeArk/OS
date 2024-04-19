#ifndef CHAT_H
#define CHAT_H

#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <file1.h>
#include <QTimer>
#include <QVBoxLayout>
#include <QCursor>
#include <QDialog>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include "kernel/kernel.h"
#include "menu.h"
#include "txt_edit.h"
#include "property.h"
#include "black_window.h"
#include "input_dialog.h"
#include "QTimer"

extern Kernel kernel;

namespace Ui {
class Folder;
}

struct FolerSet {
    QPushButton *newButton;
    QLabel *newLabel;
    bool isVisible;
    std::string my_name;
    std::string origin_name;
};

class Folder : public QWidget
{
    Q_OBJECT

private:
    int num, direction = 1;
    double time_Start;
    double time_End;
    QTimer timer;
    QTimer* update_timer;
    int next_x, next_y, folder_cnt;
    Menu *menu;

    std::vector<FolerSet> folders;
    std::string curFileName;

    FCB* myFcb;

public:
    explicit Folder(FCB* fcb, QWidget *parent = nullptr);
    ~Folder();

private slots:
    void on_pushButton_1_clicked();
    void on_back_btn_clicked();

private:
    void update_file_path();
    void init_folder();

    FCB* open_file(std::string fileName);

    void update_window();
    void handle_button_click(QPushButton *button);
    void judge_double_click();
    void display_property();
    void judge_menu_inst();
    void new_folder(bool isNewFolder, std::string fileName = "");
    void new_txt(bool isNewFolder, std::string fileName = "");
    void open_folder();
    void delete_file();
    void open_txt_editor();
    void rename();
    void compiler();
    void update_current_file_name(const std::string& fileName);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    Ui::Folder *ui;

};

#endif // CHAT_H
