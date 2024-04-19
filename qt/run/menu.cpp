#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    is_new_folder = false;
    is_rename = false;
    is_property = false;
    is_new_txt = false;
    is_compiler = false;
    is_update = false;
    is_delete = false;
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_property_btn_clicked()
{
    is_property = true;

}

void Menu::on_new_folder_btn_clicked()
{
    is_new_folder = true;
}

void Menu::on_rename_btn_clicked()
{
    is_rename = true;
}

void Menu::on_new_txt_btn_clicked()
{
    is_new_txt = true;
}

void Menu::on_compiler_btn_clicked()
{
    is_compiler = true;
}

void Menu::on_update_btn_clicked()
{
    is_update = true;
}

void Menu::on_delete_btn_clicked()
{
    is_delete = true;
}
