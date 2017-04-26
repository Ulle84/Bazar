#ifndef SELLERMANAGER_H
#define SELLERMANAGER_H

#include <QList>
#include <QString>

class Seller;

class SellerManager
{
public:
  SellerManager();
  ~SellerManager();
  void addSeller(Seller* seller);
  Seller* getSeller(int sellerNumber);

  void clear();

  bool toXml();
  bool fromXml();

private:
  QString m_fileName;
  QList<Seller*> m_sellers;
};

#endif // SELLERMANAGER_H
