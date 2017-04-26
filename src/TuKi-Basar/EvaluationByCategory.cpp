#include "ArticleManager.h"
#include "CssHelper.h"
#include "PercentageCalculator.h"

#include "EvaluationByCategory.h"
#include "ui_EvaluationByCategory.h"

EvaluationByCategory::EvaluationByCategory(ArticleManager *articleManager, QWidget *parent) :
  QWidget(parent),
  m_articleManager(articleManager),
  ui(new Ui::EvaluationByCategory)
{
  ui->setupUi(this);
}

EvaluationByCategory::~EvaluationByCategory()
{
  delete ui;
}

void EvaluationByCategory::on_pushButton_clicked()
{
  doEvaluation();
}

void EvaluationByCategory::doEvaluation()
{
  QString category = ui->lineEdit->text();

  QList<Article*> articleList = m_articleManager->getAllArticles();

  unsigned int qtyAllArticles = articleList.length();
  unsigned int qtyAllSoldArticles = 0;
  unsigned int qtyUnsoldCategoryArticles = 0;
  unsigned int qtySoldCategoryArticles = 0;

  for (QList<Article*>::const_iterator it = articleList.begin(); it != articleList.end(); it++)
  {
    bool articleIsInSearchCategory = false;
    articleIsInSearchCategory |= (*it)->m_description.contains(category, Qt::CaseInsensitive);
    articleIsInSearchCategory |= (*it)->m_size.contains(category, Qt::CaseInsensitive);

    if ((*it)->m_soldOnPc != 0)
    {
      qtyAllSoldArticles++;
      if (articleIsInSearchCategory)
      {
        qtySoldCategoryArticles++;
      }
    }
    else
    {
      if (articleIsInSearchCategory)
      {
        qtyUnsoldCategoryArticles++;
      }
    }
  }

  QString html;
  html.append("<!DOCTYPE html>");
  html.append("<html><head>");
  html.append("<meta charset=\"utf-8\">");
  html.append("<title></title>");
  html.append(CssHelper::createCssCode(false, false));
  html.append("</head><body>");

  html.append("<h1>Auswertung für die Kategorie: ");
  html.append(ui->lineEdit->text());
  html.append("</h1>");

  html.append("<table>");

  html.append("<tr>");
  html.append("<td>Gesamtzahl aller Artikel</td>");
  html.append(QString("<td>%1</td>").arg(qtyAllArticles));
  html.append("</tr>");

  html.append("<tr>");
  html.append("<td>davon verkauft</td>");
  html.append(QString("<td>%1 (%2)</td>").arg(qtyAllSoldArticles).arg(PercentageCalculator::getPercentage(qtyAllArticles, qtyAllSoldArticles)));
  html.append("</tr>");

  html.append("<tr>");
  html.append(QString("<td>Gesamtzahl aller Artikel aus der Kategorie '%1'</td>").arg(category));
  html.append(QString("<td>%1</td>").arg(qtySoldCategoryArticles + qtyUnsoldCategoryArticles));
  html.append("</tr>");

  html.append("<tr>");
  html.append(QString("<td>davon verkauft</td>").arg(category));
  html.append(QString("<td>%1 (%2)</td>").arg(qtySoldCategoryArticles).arg(PercentageCalculator::getPercentage(qtySoldCategoryArticles + qtyUnsoldCategoryArticles, qtySoldCategoryArticles)));
  html.append("</tr>");

  html.append("</table>");

  html.append("</body></html>");

  ui->webView->setHtml(html);
}

void EvaluationByCategory::on_lineEdit_returnPressed()
{
  doEvaluation();
}
