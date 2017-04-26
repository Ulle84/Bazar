#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QTextStream>
#include <QClipboard>

#include "ReservationsToExcel.h"
#include "ui_ReservationsToExcel.h"
#include "Reservation.h"

ReservationsToExcel::ReservationsToExcel(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ReservationsToExcel)
{
  ui->setupUi(this);
}

ReservationsToExcel::~ReservationsToExcel()
{
  for (auto it = mReservations.begin(); it != mReservations.end(); it++)
  {
    delete *it;
  }

  delete ui;
}

void ReservationsToExcel::on_pushButtonOpen_clicked()
{
  /*QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Vorhandene Artikel löschen?"),
                                  tr("Möchten Sie die bereits importierten Artikel löschen?"),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

    }*/

    QString dirName = QFileDialog::getExistingDirectory(this, tr("Bitte Ordner mit den Reservierungen wählen..."));

    if (dirName.isEmpty())
    {
      return;
    }

    QDir dir(dirName);

    QStringList filters;
    filters << "Reservation_*.txt";
    dir.setNameFilters(filters);

    QStringList fileNames = dir.entryList();

    unsigned int reservationCounter = 0;

    for (int i = 0; i < fileNames.length(); i++)
    {
      QString filePath = dir.filePath(fileNames.at(i));

      QFile file(filePath);

      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        continue;
      }

      QStringList fileContent;
      QTextStream in(&file);
      while (!in.atEnd())
      {
        fileContent.append(in.readLine());
      }

      if (fileContent.length() < 6)
      {
        file.close();
        continue;
      }

      Reservation* reservation = new Reservation();
      reservation->mReservationNumber = fileContent.at(0);
      reservation->mReservationDate = fileContent.at(1);
      reservation->mFirstName = fileContent.at(2);
      reservation->mLastName = fileContent.at(3);
      reservation->mEMailAdress = fileContent.at(4);
      reservation->mPhoneNumber = fileContent.at(5);

      mReservations.append(reservation);

      file.close();
      reservationCounter++;
    }

    QMessageBox mb;
    if (reservationCounter > 0)
    {
      mb.setText(tr("Es wurden erfolgreich %1 Reservierungen importiert.").arg(reservationCounter));
    }
    else
    {
      mb.setText(tr("Es wurden keine Reservierungen importiert."));
    }

    mb.exec();
}

void ReservationsToExcel::on_pushButtonToClipboard_clicked()
{
  QApplication::clipboard()->clear();
  QString table = "Vorname\tNachname\tE-Mail-Adresse\tTelefonnummer\n";

  for (auto it = mReservations.begin(); it != mReservations.end(); it++)
  {
    table.append((*it)->mFirstName + "\t");
    table.append((*it)->mLastName + "\t");
    table.append((*it)->mEMailAdress + "\t");
    table.append((*it)->mPhoneNumber + "\n");
    //table.append("%1\t%2\t%3\t%4\n").arg((*it)->mFirstName).arg((*it)->mLastName).arg((*it)->mEMailAdress).arg((*it)->mPhoneNumber);
  }
  QApplication::clipboard()->setText(table);
}
