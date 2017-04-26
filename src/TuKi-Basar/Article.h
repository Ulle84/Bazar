#ifndef ARTICLE_H
#define ARTICLE_H

#include <QString>

class Article
{
  //friend class ArticleManager;

public:
  Article(unsigned int articleNumber, unsigned int sellerNumber, unsigned int soldOnPc, double prize, double listPrize, QString size, QString description, QString soldTime);

  unsigned int m_soldOnPc;
  unsigned int m_articleNumber;
  unsigned int m_sellerNumber;
  double m_prize;
  double m_listPrize;
  QString m_size;
  QString m_description;
  QString m_soldTime;

  QString getCategory();

private:
  //TODO declare variables private again?
};

#endif // ARTICLE_H
