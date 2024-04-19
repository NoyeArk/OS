#ifndef SHUXIE_H
#define SHUXIE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

#include "kernel/kernel.h"

namespace Ui {
class EditTxt;
}

class EditTxt : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditTxt(FCB* parent_fcb, std::vector<std::string>& data, QWidget *parent = nullptr);
    ~EditTxt();

private:
    void save_txt();

private:
    FCB* myFcb;
    std::string currentFileName;
    std::string fileContentToDisplay;
    Ui::EditTxt *ui;
};

#endif // SHUXIE_H
