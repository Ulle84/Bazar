#include "ArticleManager.h"

#include <map>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QVariant>
#include <QDateTime>
#include <QStringList>

#include "Settings.h"
#include "Statistics.h"
#include "Converter.h"
#include "CssHelper.h"
#include "DateTimeHelper.h"

ArticleManager::ArticleManager(Settings* settings, QString fileName)
  : m_settings(settings),
    m_fileName(fileName)
{
  fromXml();
}

ArticleManager::~ArticleManager()
{
  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    delete *it;
  }
}

void ArticleManager::addArticle(Article* article)
{
  m_articles.append(article);
}

void ArticleManager::clear()
{
  m_articles.clear();
}

Article* ArticleManager::getArticle(int sellerNumber, int articleNumber)
{
  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); it++)
  {
    if ((*it)->m_sellerNumber == sellerNumber && (*it)->m_articleNumber == articleNumber)
    {
      return *it;
    }
  }
  return 0;
}

QList<Article *> ArticleManager::getAllArticles()
{
  return m_articles;
}

void ArticleManager::setFileName(QString fileName)
{
  m_fileName = fileName;
}

QString ArticleManager::getFileName()
{
  return m_fileName;
}

bool ArticleManager::fromXml()
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

  if (xml.name() != "TuKiBazarArticles")
  {
    return false;
  }

  while (xml.readNextStartElement())
  {
    if (xml.name() == "Article")
    {
      unsigned int articleNumber = 0;
      unsigned int sellerNumber = 0;
      unsigned int soldOnPc = 0;
      double prize = 0.0;
      double listPrize = 0.0;
      QString size;
      QString description;
      QString soldTime;

      while (xml.readNextStartElement())
      {
        if (xml.name() == "Seller")
        {
          sellerNumber = xml.readElementText().toInt();
        }
        else if (xml.name() == "Number")
        {
          articleNumber = xml.readElementText().toInt();
        }
        else if (xml.name() == "Prize")
        {
          prize = xml.readElementText().toDouble();
        }
        else if (xml.name() == "ListPrize")
        {
          listPrize = xml.readElementText().toDouble();
        }
        else if (xml.name() == "Size")
        {
          size = xml.readElementText();
        }
        else if (xml.name() == "Description")
        {
          description = xml.readElementText();
        }
        else if (xml.name() == "SoldOnPc")
        {
          soldOnPc = xml.readElementText().toInt();
        }
        else if (xml.name() == "SoldTime")
        {
          soldTime = xml.readElementText();
        }
        else
        {
          xml.skipCurrentElement();
        }
      }

      Article* article = new Article(articleNumber, sellerNumber, soldOnPc, prize, listPrize, size, description, soldTime);
      addArticle(article);

      if (soldOnPc == m_settings->getPc() && !soldTime.isEmpty())
      {
        m_transactions[soldTime].append(article);
      }
    }
    else
    {
      xml.skipCurrentElement();
    }
  }

  return true;
}

bool ArticleManager::toXml()
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

  xml.writeStartElement("TuKiBazarArticles");

  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    xml.writeStartElement("Article");
    xml.writeTextElement("Seller", QString("%1").arg((*it)->m_sellerNumber));
    xml.writeTextElement("Number", QString("%1").arg((*it)->m_articleNumber));
    xml.writeTextElement("Prize", QString("%1").arg((*it)->m_prize));
    xml.writeTextElement("ListPrize", QString("%1").arg((*it)->m_listPrize));
    xml.writeTextElement("Size", (*it)->m_size);
    xml.writeTextElement("Description", (*it)->m_description);
    xml.writeTextElement("SoldOnPc", QString("%1").arg((*it)->m_soldOnPc));
    xml.writeTextElement("SoldTime", (*it)->m_soldTime);
    xml.writeEndElement(); // Article
  }

  xml.writeEndElement(); // TuKiBazarArticles
  xml.writeEndDocument();

  file.close();

  return true;
}

void ArticleManager::resetCurrentSale()
{
  m_currentSale.clear();
}

void ArticleManager::addArticleToCurrentSale(Article* article)
{
  m_currentSale.append(article);
}

void ArticleManager::removeLastArticleFromCurrentSale()
{
  if (m_currentSale.length() > 0)
  {
    m_currentSale.removeLast();
  }
}

void ArticleManager::finishCurrentSale(unsigned int pcNumber)
{
  QString soldTime = DateTimeHelper::getCurrentDateAndTime();

  for (QList<Article*>::iterator it = m_currentSale.begin(); it != m_currentSale.end(); ++it)
  {
    (*it)->m_soldOnPc = pcNumber;
    (*it)->m_soldTime = soldTime;
  }

  m_transactions[soldTime] = m_currentSale;

  resetCurrentSale();
  toXml();
}

bool ArticleManager::isCurrentSaleEmpty()
{
  return m_currentSale.isEmpty();
}

bool ArticleManager::isArticleInCurrentSale(unsigned int sellerNumber, unsigned int articleNumber)
{
  for (QList<Article*>::iterator it = m_currentSale.begin(); it != m_currentSale.end(); ++it)
  {
    if ((*it)->m_sellerNumber == sellerNumber && (*it)->m_articleNumber == articleNumber)
    {
      return true;
    }
  }
  return false;
}

Article *ArticleManager::getLastArticleInCurrentSale()
{
  Article* article = 0;
  if (m_currentSale.length() > 0)
  {
    article = m_currentSale.last();
  }
  return article;
}

QString ArticleManager::currentSaleToText()
{
  if (m_currentSale.length() < 1)
  {
    return QString();
  }

  double sum = 0;

  QString currentSaleText = "Ver  Art  Preis   Beschreibung\n";
  for (QList<Article*>::iterator it = m_currentSale.begin(); it != m_currentSale.end(); ++it)
  {
    currentSaleText += QString("%1  %2  %3  %4\n").arg((*it)->m_sellerNumber).arg((*it)->m_articleNumber).arg(Converter::prizeToString((*it)->m_prize)).arg((*it)->m_description);
    sum += (*it)->m_prize;
  }
  currentSaleText += "\n";
  currentSaleText += QString("Summe:    %1").arg(Converter::prizeToString(sum));
  return currentSaleText;
}

QString ArticleManager::currentSaleToHtml()
{
  QString currentSaleHtml;

  if (m_currentSale.empty())
  {
    return currentSaleHtml;
  }

  currentSaleHtml += "<!DOCTYPE html>";
  currentSaleHtml += "<meta charset=\"utf-8\">";
  currentSaleHtml += "<html>";
  currentSaleHtml += CssHelper::createCssCode(true);
  currentSaleHtml += "<table>";
  currentSaleHtml += "<tr>";
  currentSaleHtml += "<th>Verkäufer</th>";
  currentSaleHtml += "<th>Artikel</th>";
  currentSaleHtml += "<th>Preis</th>";
  currentSaleHtml += "<th>Beschreibung</th>";
  currentSaleHtml += "</tr>";

  for (QList<Article*>::iterator it = m_currentSale.begin(); it != m_currentSale.end(); ++it)
  {
    currentSaleHtml += "<tr>";
    currentSaleHtml += QString("<td>%1</td>").arg((*it)->m_sellerNumber);
    currentSaleHtml += QString("<td>%1</td>").arg((*it)->m_articleNumber);
    currentSaleHtml += QString("<td class=\"prize\">%1 &euro;</td>").arg(Converter::prizeToString((*it)->m_prize));
    currentSaleHtml += QString("<td>%1</td>").arg((*it)->m_description);
    currentSaleHtml += "</tr>";
  }

  currentSaleHtml += "</table>";
  currentSaleHtml += "</html>";

  return currentSaleHtml;
}

double ArticleManager::getSumOfCurrentSale()
{
  double sum = 0;
  if (m_currentSale.length() < 1)
  {
    return sum;
  }

  for (QList<Article*>::iterator it = m_currentSale.begin(); it != m_currentSale.end(); ++it)
  {
    sum += (*it)->m_prize;
  }

  return sum;
}

std::map<int, double> ArticleManager::getSalesPerSeller()
{
  std::map<int, double> map;

  for (int i = m_settings->getSellerMin(); i <= m_settings->getSellerMax(); i++)
  {
    map[i] = 0.0;
  }

  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    if ((*it)->m_soldOnPc == 0)
    {
      continue;
    }

    if (map.find((*it)->m_sellerNumber) == map.end())
    {
      // this should not happen, since we check the seller number at input
      map[(*it)->m_sellerNumber] = 0.0;
    }

    map[(*it)->m_sellerNumber] += (*it)->m_prize;
  }

  return map;
}

std::map<int, double> ArticleManager::getSoldArticles(int sellerNumber)
{
  std::map<int, double> map;

  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    if ((*it)->m_soldOnPc == 0)
    {
      continue;
    }

    if ((*it)->m_sellerNumber != sellerNumber)
    {
      continue;
    }

    map[(*it)->m_articleNumber] = (*it)->m_prize;
  }

  return map;
}

std::vector<int> ArticleManager::getUnsoldArticles(int sellerNumber)
{
  std::vector<int> vector;

  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    if ((*it)->m_soldOnPc != 0)
    {
      continue;
    }

    if ((*it)->m_sellerNumber != sellerNumber)
    {
      continue;
    }

    vector.push_back((*it)->m_articleNumber);
  }

  return vector;
}

std::map<int, int> ArticleManager::getSoldArticlesPerSeller()
{
  std::map<int, int> map;

  for (int i = m_settings->getSellerMin(); i <= m_settings->getSellerMax(); i++)
  {
    map[i] = 0;
  }

  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    if ((*it)->m_soldOnPc == 0)
    {
      continue;
    }

    if (map.find((*it)->m_sellerNumber) == map.end())
    {
      // this should not happen, since we check the seller number at input
      map[(*it)->m_sellerNumber] = 0;
    }

    map[(*it)->m_sellerNumber]++;
  }

  return map;
}

std::map<int, int> ArticleManager::getOfferedArticlesPerSeller()
{
  std::map<int, int> map;

  for (int i = m_settings->getSellerMin(); i <= m_settings->getSellerMax(); i++)
  {
    map[i] = 0;
  }

  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    if (map.find((*it)->m_sellerNumber) == map.end())
    {
      // this should not happen, since we check the seller number at input
      map[(*it)->m_sellerNumber] = 0;
    }

    map[(*it)->m_sellerNumber]++;
  }

  return map;
}

std::map<QString, int> ArticleManager::getSoldArticlesInRanges()
{
  //TODO think about performance

  std::map<QString, int> output;

  int maxCountOfArticles = m_settings->getArticleMax() - m_settings->getArticleMin() + 1;

  for (int i = 0; i <= maxCountOfArticles; i += 10)
  {
    output[QString("%1 - %2").arg(Converter::intToQString(i)).arg(Converter::intToQString(i + 9))] = 0;
  }

  std::map<int, int> soldArticlesPerSeller = getSoldArticlesPerSeller();

  for (std::map<int, int>::iterator it = soldArticlesPerSeller.begin(); it != soldArticlesPerSeller.end(); ++it)
  {
    int value = it->second / 10;
    output[QString("%1 - %2").arg(Converter::intToQString(value * 10)).arg(Converter::intToQString(value * 10 + 9))]++;
  }

  return output;
}

std::map<QString, int> ArticleManager::getOfferedArticlesInRanges()
{
  //TODO think about performance

  std::map<QString, int> output;

  int maxCountOfArticles = m_settings->getArticleMax() - m_settings->getArticleMin() + 1;

  for (int i = 0; i <= maxCountOfArticles; i += 10)
  {
    output[QString("%1 - %2").arg(Converter::intToQString(i)).arg(Converter::intToQString(i + 9))] = 0;
  }

  std::map<int, int> offeredArticlesPerSeller = getOfferedArticlesPerSeller();

  for (std::map<int, int>::iterator it = offeredArticlesPerSeller.begin(); it != offeredArticlesPerSeller.end(); ++it)
  {
    int value = it->second / 10;
    output[QString("%1 - %2").arg(Converter::intToQString(value * 10)).arg(Converter::intToQString(value * 10 + 9))]++;
  }

  return output;
}

void ArticleManager::sync(ArticleManager *other)
{
  for (QList<Article*>::iterator it = other->m_articles.begin(); it != other->m_articles.end(); it++)
  {
    if ((*it)->m_soldOnPc != 0)
    {
      Article* article = getArticle((*it)->m_sellerNumber, (*it)->m_articleNumber);
      if (article != 0)
      {
        if (article->m_soldOnPc == 0)
        {
          article->m_soldOnPc = (*it)->m_soldOnPc;
          article->m_soldTime = (*it)->m_soldTime;
          article->m_prize = (*it)->m_prize;
        }
        else
        {
          // TODO article was also sold on this PC, what shall we do? which prize shall we take?
        }
      }
      else
      {
        // append article also to this list
        Article* newArticle = new Article((*it)->m_articleNumber, (*it)->m_sellerNumber, (*it)->m_soldOnPc, (*it)->m_prize, (*it)->m_listPrize, (*it)->m_size, (*it)->m_description, (*it)->m_soldTime);
        m_articles.append(newArticle);
      }
    }
  }
}

double ArticleManager::getPayOutFactor()
{
  return (100.0 - m_settings->getDeductionPercentage()) / 100.0;
}

int ArticleManager::getCountOfTransactions()
{
  return m_transactions.size();
}

int ArticleManager::getCountOfSoldArticles()
{
  int count = 0;
  for (QMap<QString, QList<Article*> >::iterator it = m_transactions.begin(); it != m_transactions.end(); ++it)
  {
    count += it.value().size();
  }
  return count;
}

QMap<QString, QList<Article*> > ArticleManager::getTransactions()
{
  return m_transactions;
}

bool ArticleManager::returnArticle(int sellerNumber, int articleNumber)
{
  for (QList<Article*>::iterator it = m_currentSale.begin(); it != m_currentSale.end(); ++it)
  {
    if ((*it)->m_articleNumber == articleNumber && (*it)->m_sellerNumber == sellerNumber)
    {
      (*it)->m_soldOnPc = 0;
      (*it)->m_soldTime = "";
      return true;
    }
  }

  for (QMap<QString, QList<Article*> >::iterator it = m_transactions.begin(); it != m_transactions.end(); ++it)
  {
    for (QList<Article*>::iterator it2 = it.value().begin(); it2 != it.value().end(); ++it2)
    {
      if ((*it2)->m_articleNumber == articleNumber && (*it2)->m_sellerNumber == sellerNumber)
      {
        (*it2)->m_soldOnPc = 0;
        (*it2)->m_soldTime = "";
        return true;
      }
    }
  }

  return false;
}

void ArticleManager::sellAllArticles()
{
  for (QList<Article*>::iterator it = m_articles.begin(); it != m_articles.end(); ++it)
  {
    (*it)->m_soldOnPc = m_settings->getPc();
    (*it)->m_soldTime = DateTimeHelper::getCurrentDateAndTime();
  }
}
