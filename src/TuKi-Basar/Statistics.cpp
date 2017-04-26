#include <QStringList>

#include "Article.h"

#include "Statistics.h"

Statistics::Statistics()
{
  resetValues();
}

void Statistics::createStatistics(const QList<Article *> articleList, int deductionPercentage)
{
  resetValues();

  m_deductionPercentage = deductionPercentage;

  QStringList transactions;

  for (QList<Article*>::const_iterator it = articleList.begin(); it != articleList.end(); it++)
  {
    if ((*it)->m_soldOnPc != 0)
    {
      QString transaction = QString("%1 %2").arg((*it)->m_soldOnPc).arg((*it)->m_soldTime);

      m_volumeOfSale += (*it)->m_prize;
      (m_qtySoldArticles)++;

      if ((*it)->m_size.length() > 1)
      {
        m_qtySoldClothes++;
      }
      else
      {
        m_qtySoldMisc++;
      }

      if (!transactions.contains(transaction))
      {
        transactions.append(transaction);
      }
    }
    else
    {
      // TODO refactor -> here we have doubled code - see some lines below
      if ((*it)->m_size.length() > 1)
      {
        m_qtyClothes++;
      }
      else
      {
        m_qtyMisc++;
      }
    }
  }

  m_qtySales = transactions.length();

  if (m_qtySales == 0)
  {
    m_articlesPerSale = 0;
  }
  else
  {
    m_articlesPerSale = m_qtySoldArticles * 1.0 / m_qtySales;
  }
  m_deduction = m_volumeOfSale * m_deductionPercentage / 100.0;

  m_qtyAllArticles = articleList.length();

  if (m_qtyAllArticles == 0)
  {
    m_percentageOfSoldArticles = 0.0;
  }
  else
  {
    m_percentageOfSoldArticles = m_qtySoldArticles * 100.0 / m_qtyAllArticles;
  }
}

int Statistics::getQtySales()
{
  return m_qtySales;
}

int Statistics::getQtySoldArticles()
{
  return m_qtySoldArticles;
}

int Statistics::getQtyAllArticles()
{
  return m_qtyAllArticles;
}

int Statistics::getQtyClothes()
{
  return m_qtyClothes;
}

int Statistics::getQtyMisc()
{
  return m_qtyMisc;
}

int Statistics::getQtySoldClothes()
{
  return m_qtySoldClothes;
}

int Statistics::getQtySoldMisc()
{
  return m_qtySoldMisc;
}

double Statistics::getVolumeOfSale()
{
  return m_volumeOfSale;
}

double Statistics::getDeductionPercentage()
{
  return m_deductionPercentage;
}

double Statistics::getDeduction()
{
  return m_deduction;
}

double Statistics::getArticlesPerSale()
{
  return m_articlesPerSale;
}

double Statistics::getPercentageOfSoldArticles()
{
  return m_percentageOfSoldArticles;
}

void Statistics::resetValues()
{
  m_volumeOfSale = 0.0;
  m_deduction = 0.0;
  m_deductionPercentage = 0.0;
  m_articlesPerSale = 0.0;
  m_percentageOfSoldArticles = 0.0;

  m_qtySales = 0;
  m_qtySoldArticles = 0;
  m_qtyAllArticles = 0;
  m_qtySoldClothes = 0;
  m_qtySoldMisc = 0;
  m_qtyClothes = 0;
  m_qtyMisc = 0;
}
