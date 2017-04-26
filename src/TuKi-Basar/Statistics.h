#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMap>
#include <QList>
#include <QPair>
#include <QString>

class Article;

class Statistics
{
public:
  Statistics();

  void createStatistics(const QList<Article*> articleList, int deductionPercentage);

  int getQtySales();
  int getQtySoldArticles();
  int getQtyAllArticles();
  int getQtyClothes();
  int getQtyMisc();
  int getQtySoldClothes();
  int getQtySoldMisc();

  double getVolumeOfSale();
  double getDeductionPercentage();
  double getDeduction();
  double getArticlesPerSale();
  double getPercentageOfSoldArticles();

private:
  void resetValues();

  double m_volumeOfSale;
  double m_deduction;
  double m_deductionPercentage;
  double m_articlesPerSale;
  double m_percentageOfSoldArticles;

  int m_qtySales;
  int m_qtySoldArticles;
  int m_qtyAllArticles;
  int m_qtyClothes;
  int m_qtyMisc;
  int m_qtySoldClothes;
  int m_qtySoldMisc;

  QMap<QString /*categoryName*/, QPair<unsigned int /*soldArticles*/, unsigned int /*unsoldeArticles*/> > m_categories;
};

#endif // STATISTICS_H
