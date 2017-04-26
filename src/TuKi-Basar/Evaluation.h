#ifndef EVALUATION_H
#define EVALUATION_H

#include <QDialog>

class ArticleManager;
class Settings;
class EvaluationView;
class Statistics;
class SellerManager;
class ExcelExport;
class EvaluationByCategory;

namespace Ui {
class Evaluation;
}

class Evaluation : public QDialog
{
  Q_OBJECT

public:
  explicit Evaluation(ArticleManager* articleManager, Settings* settings, SellerManager* sellerManager, QWidget *parent = 0);
  ~Evaluation();

  void doEvaluation();

private:
  void updateWebViews();
  QString createHtmlCodeOverview();
  QString createHtmlCodeSoldArticles();
  QString createHtmlCodeUnsoldArticles();

  void appendSoldArticleHeader(QString& html, bool columnDiscrepancyNeeded);

  Ui::Evaluation *ui;
  ArticleManager* m_articleManager;
  SellerManager* m_sellerManager;
  Settings* m_settings;
  EvaluationView* m_overview;
  EvaluationView* m_soldArticles;
  EvaluationView* m_unsoldArticles;
  Statistics* m_statistics;
  ExcelExport* m_excelExport;
  EvaluationByCategory* m_evaluationByCategory;
};

#endif // EVALUATION_H
