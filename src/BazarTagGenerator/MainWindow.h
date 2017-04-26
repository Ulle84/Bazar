#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

private slots:
  void on_webView_loadFinished(bool loadFinished);
  void on_pushButtonStart_clicked();

private:
  bool toXml();
  bool fromXml();

  Ui::MainWindow* ui;
  QString m_fileName;
};

#endif // MAINWINDOW_H
