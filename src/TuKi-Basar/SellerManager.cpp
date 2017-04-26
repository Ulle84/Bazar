#include "SellerManager.h"
#include "Seller.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

SellerManager::SellerManager()
{
  m_fileName = "Sellers.xml";
  fromXml();
}

SellerManager::~SellerManager()
{
  toXml();
  for (QList<Seller*>::iterator it = m_sellers.begin(); it != m_sellers.end(); ++it)
  {
    delete *it;
  }
}

void SellerManager::addSeller(Seller *seller)
{
  m_sellers.append(seller);
}

Seller* SellerManager::getSeller(int sellerNumber)
{
  for (QList<Seller*>::iterator it = m_sellers.begin(); it != m_sellers.end(); ++it)
  {
    if ((*it)->m_sellerNumber == sellerNumber)
    {
      return *it;
    }
  }
  return 0;
}

void SellerManager::clear()
{
  m_sellers.clear();
}

bool SellerManager::toXml()
{
  QFile file(m_fileName);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    return false;
  }

  QXmlStreamWriter xml(&file);
  xml.setAutoFormatting(true);
  xml.setAutoFormattingIndent(2);
  xml.writeStartDocument();

  xml.writeStartElement("TuKiBazarSellers");

  for (QList<Seller*>::iterator it = m_sellers.begin(); it != m_sellers.end(); ++it)
  {
    xml.writeStartElement("Seller");
    xml.writeTextElement("SellerNumber", QString::number((*it)->m_sellerNumber));
    xml.writeTextElement("FirstName", (*it)->m_firstName);
    xml.writeTextElement("LastName", (*it)->m_lastName);
    xml.writeTextElement("Phone", (*it)->m_phone);
    xml.writeEndElement(); // Seller
  }

  xml.writeEndElement(); // TuKiBazarSellers
  xml.writeEndDocument();

  file.close();

  return true;
}

bool SellerManager::fromXml()
{
  QFile file(m_fileName);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return false;
  }

  QXmlStreamReader xml(&file);

  if (!xml.readNextStartElement())
  {
    return false;
  }

  if (xml.name() != "TuKiBazarSellers")
  {
    return false;
  }

  while (xml.readNextStartElement())
  {
    if (xml.name() == "Seller")
    {
      unsigned int sellerNumber = 0;
      QString firstName;
      QString lastName;
      QString phone;

      while (xml.readNextStartElement())
      {
        if (xml.name() == "SellerNumber")
        {
          sellerNumber = xml.readElementText().toInt();
        }
        else if (xml.name() == "FirstName")
        {
          firstName = xml.readElementText();
        }
        else if (xml.name() == "LastName")
        {
          lastName = xml.readElementText();
        }
        else if (xml.name() == "Phone")
        {
          phone = xml.readElementText();
        }
        else
        {
          xml.skipCurrentElement();
        }
      }

      Seller* seller = new Seller(sellerNumber, firstName, lastName, phone);
      addSeller(seller);
    }
    else
    {
      xml.skipCurrentElement();
    }
  }

  return true;
}
