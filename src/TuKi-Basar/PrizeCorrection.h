#ifndef PRIZECORRECTION_H
#define PRIZECORRECTION_H

#include <QDialog>

namespace Ui {
class PrizeCorrection;
}

class PrizeCorrection : public QDialog
{
  Q_OBJECT

public:
  explicit PrizeCorrection(QWidget *parent = 0);
  ~PrizeCorrection();
  void setPrize(double prize);
  double getPrize();

private slots:
  void on_pushButtonOK_clicked();
  void on_pushButtonCancel_clicked();

private:
  Ui::PrizeCorrection *ui;
};

#endif // PRIZECORRECTION_H
