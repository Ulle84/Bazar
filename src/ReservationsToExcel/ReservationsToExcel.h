#ifndef RESERVATIONSTOEXCEL_H
#define RESERVATIONSTOEXCEL_H

#include <QWidget>

class Reservation;

namespace Ui {
class ReservationsToExcel;
}

class ReservationsToExcel : public QWidget
{
  Q_OBJECT

public:
  explicit ReservationsToExcel(QWidget *parent = 0);
  ~ReservationsToExcel();

private slots:
  void on_pushButtonOpen_clicked();
  void on_pushButtonToClipboard_clicked();

private:
  Ui::ReservationsToExcel *ui;

  QList<Reservation*> mReservations;
};

#endif // RESERVATIONSTOEXCEL_H
