#ifndef INPUT_DIALOG_H
#define INPUT_DIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class InputDialog : public QDialog {
    Q_OBJECT

public:
    InputDialog(QWidget *parent = nullptr);

    QString getText() const;

private:
    QLineEdit *inputLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // INPUT_DIALOG_H
