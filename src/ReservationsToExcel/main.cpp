#include "ReservationsToExcel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ReservationsToExcel w;
  w.show();

  return a.exec();
}
