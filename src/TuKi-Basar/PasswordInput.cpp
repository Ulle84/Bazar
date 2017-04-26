#include "PasswordInput.h"
#include "ui_PasswordInput.h"

PasswordInput::PasswordInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordInput)
{
    ui->setupUi(this);
}

PasswordInput::~PasswordInput()
{
    delete ui;
}

QString PasswordInput::getPassword()
{
    return ui->lineEditPassword->text();
}
