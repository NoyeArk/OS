#ifndef FILE1_H
#define FILE1_H

#include <QWidget>
#include <QMainWindow>
namespace Ui {
class file1;
}

class file1 : public QWidget
{
    Q_OBJECT

public:
    explicit file1(QWidget *parent = nullptr);
    ~file1();

private:
    Ui::file1 *ui;
};

#endif // FILE1_H
