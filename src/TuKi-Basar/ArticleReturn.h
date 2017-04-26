#ifndef ARTICLERETURN_H
#define ARTICLERETURN_H

#include <QDialog>

namespace Ui {
class ArticleReturn;
}

class ArticleReturn : public QDialog
{
    Q_OBJECT

public:
    explicit ArticleReturn(QWidget *parent = 0);
    ~ArticleReturn();

  int getSellerNumber();
  int getArticleNumber();

private slots:
  void on_buttonBox_accepted();

private:
    Ui::ArticleReturn *ui;

    int m_sellerNumber;
    int m_articleNumber;
};

#endif // ARTICLERETURN_H
