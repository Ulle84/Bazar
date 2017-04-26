#ifndef SELLER_H
#define SELLER_H

#include <QString>

class Seller
{
public:
  Seller(int sellerNumber, const QString& firstName, const QString& lastName, const QString& phone);

  //TODO define members private -> declare friends
  int m_sellerNumber;
  QString m_firstName;
  QString m_lastName;
  QString m_phone;
};

#endif // SELLER_H
