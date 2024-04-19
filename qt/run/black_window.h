#ifndef BLACK_WINDOW_H
#define BLACK_WINDOW_H

#include <QWidget>
#include <Qpainter>
#include <QTextStream>
#include <QFile>
#include <QDebug>

#include "compiler/include/compiler.h"

namespace Ui {
class BlackWindow;
}

extern Compiler compiler;

class BlackWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BlackWindow(std::string code, QWidget *parent = nullptr);
    ~BlackWindow();
    void paintEvent(QPaintEvent * event);

private:
    Ui::BlackWindow *ui;
};

#endif // BLACK_WINDOW_H
