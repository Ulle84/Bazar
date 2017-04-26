#include <QMessageBox>
#include <QRegExp>

#include "ArticleReturn.h"
#include "ui_ArticleReturn.h"

ArticleReturn::ArticleReturn(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ArticleReturn),
  m_sellerNumber(0),
  m_articleNumber(0)
{
  ui->setupUi(this);

  QRegExp rx ("[0-9]{6}");
  ui->lineEdit->setValidator(new QRegExpValidator (rx, this));
}

ArticleReturn::~ArticleReturn()
{
  delete ui;
}

int ArticleReturn::getSellerNumber()
{
  return m_sellerNumber;
}

int ArticleReturn::getArticleNumber()
{
  return m_articleNumber;
}

void ArticleReturn::on_buttonBox_accepted()
{
  QString input = ui->lineEdit->text();

  if (input.length() != 6)
  {
    QMessageBox mb;
    mb.setText(tr("Die Eingabe ist fehlerhaft!"));
    mb.exec();
    ui->lineEdit->selectAll();
    return;
  }

  bool conversion1 = false;
  bool conversion2 = false;

  m_sellerNumber = input.left(3).toInt(&conversion1);
  m_articleNumber = input.right(3).toInt(&conversion2);

  if (!conversion1 || !conversion2)
  {
    QMessageBox mb;
    mb.setText(tr("Die Eingabe ist fehlerhaft!"));
    mb.exec();
    ui->lineEdit->selectAll();
    return;
  }

  accept();
}
