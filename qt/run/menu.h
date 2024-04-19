#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QDebug>


namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT
public:
    bool is_new_folder;
    bool is_new_txt;
    bool is_rename;
    bool is_property;
    bool is_compiler;
    bool is_update;
    bool is_delete;

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
    void on_property_btn_clicked();
    void on_new_folder_btn_clicked();
    void on_rename_btn_clicked();
    void on_new_txt_btn_clicked();
    void on_compiler_btn_clicked();
    void on_update_btn_clicked();
    void on_delete_btn_clicked();

private:
    Ui::Menu *ui;
};

#endif // MENU_H
