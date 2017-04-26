#include "PrizeCorrection.h"
#include "ui_PrizeCorrection.h"

#include <QMessageBox>

PrizeCorrection::PrizeCorrection(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PrizeCorrection)
{
  ui->setupUi(this);
}

PrizeCorrection::~PrizeCorrection()
{
  delete ui;
}

void PrizeCorrection::setPrize(double prize)
{
  ui->doubleSpinBoxPrize->setValue(prize);
  ui->doubleSpinBoxPrize->setFocus();
  ui->doubleSpinBoxPrize->selectAll();
}

double PrizeCorrection::getPrize()
{
  return ui->doubleSpinBoxPrize->value();
}

void PrizeCorrection::on_pushButtonOK_clicked()
{
  double input = ui->doubleSpinBoxPrize->value();
  if (input >= 25)
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Hoher Preis"),
                                  tr("Möchten Sie wirklich den Preis von %1 Euro übernehmen?").arg(input),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
      this->accept();
    }
  }
  else if (input < 0.01)
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Preis von Null Euro"),
                                  tr("Möchten Sie wirklich den Preis von %1 Euro übernehmen?").arg(input),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
      this->accept();
    }
  }
  else
  {
    this->accept();
  }
}

void PrizeCorrection::on_pushButtonCancel_clicked()
{
    this->reject();
}
