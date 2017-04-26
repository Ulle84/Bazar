#ifndef TUKIBASAR_H
#define TUKIBASAR_H

#include <QAction>
#include <QList>
#include <QMainWindow>

class Article;
class ArticleManager;
class Evaluation;
class Settings;
class SellerManager;

namespace Ui {
class TuKiBasar;
}

class TuKiBasar : public QMainWindow
{
  Q_OBJECT

public:
  explicit TuKiBasar(QWidget *parent = 0);
  ~TuKiBasar();

private slots:
  void on_actionArticleReturn_triggered();
  void on_actionSettings_triggered();
  void on_actionEvaluation_triggered();
  void on_actionImportArticleLists_triggered();
  void on_actionCompleteEvaluation_triggered();
  void on_actionExportSoldArticles_triggered();
  void on_actionActivateAdvancedAccess_triggered();
  void on_actionDeactivateAdvancedAccess_triggered();

  void on_doubleSpinBoxMoneyGiven_valueChanged(double moneyGiven);

  void on_lineEditInput_returnPressed();

  void on_pushButtonDeleteLastInput_clicked();
  void on_pushButtonNextCustomer_clicked();
  void on_pushButtonCorrectPrize_clicked();
  void on_pushButtonSalesHistorie_clicked();

private:
  void closeEvent(QCloseEvent *event);
  void setLastArticleInformation();
  void clearLastArticleInformation();
  void updateArticleView();
  void askUserToFinishCurrentSale();
  void prepareForNextInput();
  bool checkPassword();
  void calculateChange();
  void setCashPointNumber();
  void updateInformation();
  void setPasswordProtectedActionsVisible(bool visible);

  Ui::TuKiBasar *ui;
  Settings* m_settings;
  Evaluation* m_evaluation;
  ArticleManager* m_articleManager;
  SellerManager* m_sellerManager;
  QList<QAction*> m_passwortProtectedActions;
};

#endif // TUKIBASAR_H
