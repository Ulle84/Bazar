#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = 0);
  ~Settings();

  int getPc();
  int getArticleMin();
  int getArticleMax();
  int getSellerMin();
  int getSellerMax();
  double getDeductionPercentage();

  void setPc(int value);
  void setArticleMin(int value);
  void setArticleMax(int value);
  void setSellerMin(int value);
  void setSellerMax(int value);
  void setDeductionPercentage(double value);

  bool fromXml();
  bool toXml();

private slots:
  void on_pushButtonOk_clicked();

private:
  Ui::Settings *ui;
  QString m_fileName;
};

#endif // SETTINGS_H
