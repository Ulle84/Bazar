#include "Seller.h"

Seller::Seller(int sellerNumber, const QString& firstName, const QString& lastName, const QString& phone)
  : m_sellerNumber(sellerNumber),
    m_firstName(firstName),
    m_lastName(lastName),
    m_phone(phone)
{
}
