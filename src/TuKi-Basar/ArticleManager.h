#ifndef ARTICLEMANAGER_H
#define ARTICLEMANAGER_H

#include <map>
#include <vector>

#include <QList>
#include <QMap>
#include <QString>

#include "Article.h"

class Settings;

class ArticleManager
{
public:
  ArticleManager(Settings* settings, QString fileName);
  ~ArticleManager();

  void addArticle(Article* article);
  void clear();
  Article* getArticle(int sellerNumber, int articleNumber);
  QList<Article*> getAllArticles();

  void setFileName(QString fileName);
  QString getFileName();

  bool fromXml();
  bool toXml();

  void resetCurrentSale();
  void addArticleToCurrentSale(Article* article);
  void removeLastArticleFromCurrentSale();
  void finishCurrentSale(unsigned int pcNumber);
  bool isCurrentSaleEmpty();
  bool isArticleInCurrentSale(unsigned int sellerNumber, unsigned int articleNumber);
  Article* getLastArticleInCurrentSale();
  QString currentSaleToText();
  QString currentSaleToHtml();
  double getSumOfCurrentSale();

  std::map<int, double> getSalesPerSeller();
  std::map<int, int> getSoldArticlesPerSeller();
  std::map<int, int> getOfferedArticlesPerSeller();
  std::map<QString, int> getSoldArticlesInRanges();
  std::map<QString, int> getOfferedArticlesInRanges();
  std::map<int, double> getSoldArticles(int sellerNumber);
  std::vector<int> getUnsoldArticles(int sellerNumber);
  void sync(ArticleManager* other);

  double getPayOutFactor();
  int getCountOfTransactions();
  int getCountOfSoldArticles();

  QMap<QString, QList<Article*> > getTransactions();

  bool returnArticle(int sellerNumber, int articleNumber);

  void sellAllArticles(); // this ist for testing purposes only!!!

private:
  QList<Article*> m_articles;
  QList<Article*> m_currentSale;
  QMap<QString, QList<Article*> > m_transactions;
  QString m_fileName;
  Settings* m_settings;
};

#endif // ARTICLEMANAGER_H
