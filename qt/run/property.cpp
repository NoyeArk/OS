#include "property.h"
#include "ui_property.h"

Property::Property(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Property)
{
    ui->setupUi(this);
}

Property::~Property()
{
    delete ui;
}

void Property::set_creat_time(const std::string& content)
{
    ui->creat_time_content_label->setText(QString::fromStdString(content));
}

void Property::set_update_time(const std::string& content)
{
    ui->update_time_content_label->setText(QString::fromStdString(content));
}

void Property::set_location(const std::string& content)
{
    ui->location_content_label->setText(QString::fromStdString(content));
}

void Property::set_size(const std::string& content)
{
    ui->size_content_label->setText(QString::fromStdString(content));
}

void Property::set_type(const std::string& content)
{
    ui->type_content_label->setText(QString::fromStdString(content));
}

void Property::set_property(const std::string& content)
{
    ui->property_content_label->setText(QString::fromStdString(content));
}
