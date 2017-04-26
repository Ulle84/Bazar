#ifndef PASSWORDINPUT_H
#define PASSWORDINPUT_H

#include <QDialog>

namespace Ui {
class PasswordInput;
}

class PasswordInput : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordInput(QWidget *parent = 0);
    ~PasswordInput();
    QString getPassword();

private:
    Ui::PasswordInput *ui;
};

#endif // PASSWORDINPUT_H
