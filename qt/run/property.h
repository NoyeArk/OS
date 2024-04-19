#ifndef PROPERTY_H
#define PROPERTY_H

#include <QWidget>

namespace Ui {
class Property;
}

class Property : public QWidget
{
    Q_OBJECT

public:
    explicit Property(QWidget *parent = nullptr);
    ~Property();

    void set_creat_time(const std::string& content);
    void set_update_time(const std::string& content);
    void set_location(const std::string& content);
    void set_size(const std::string& content);
    void set_type(const std::string& content);
    void set_property(const std::string& content);

private:
    Ui::Property *ui;
};

#endif // PROPERTY_H
