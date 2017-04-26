#ifndef RESERVATION_H
#define RESERVATION_H

#include <QString>

class Reservation
{
public:
  Reservation();

  QString mReservationNumber;
  QString mReservationDate;
  QString mFirstName;
  QString mLastName;
  QString mEMailAdress;
  QString mPhoneNumber;
};

#endif // RESERVATION_H
