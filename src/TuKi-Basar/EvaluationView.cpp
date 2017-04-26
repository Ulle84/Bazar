#if QT_VERSION >= 0x050000
    #include <QtPrintSupport/QPrinter>
    #include <QtWebKitWidgets/QWebFrame>
#else
    #include <QPrinter>
    #include <QWebFrame>
#endif

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QFileDialog>

#include "EvaluationView.h"
#include "ui_EvaluationView.h"
#include "DateTimeHelper.h"

EvaluationView::EvaluationView(const QString& name, QWidget *parent) :
  QWidget(parent),
  m_name(name),
  ui(new Ui::EvaluationView)
{
  ui->setupUi(this);
}

EvaluationView::~EvaluationView()
{
  delete ui;
}

void EvaluationView::setName(const QString &name)
{
  m_name = name;
}

QString EvaluationView::getName()
{
  return m_name;
}

void EvaluationView::setHtmlContent(QString htmlContent)
{
  ui->webView->setHtml(htmlContent);
}

void EvaluationView::on_webView_loadFinished(bool loadFinished)
{
  if (!loadFinished)
  {
    return;
  }
  ui->pushButton->setEnabled(true);
}

void EvaluationView::on_pushButton_clicked()
{
  QString outputFile = QFileDialog::getSaveFileName(this, tr("Bitte geben Sie den Dateinamen an"), QString("%1_%2.pdf").arg(m_name).arg(DateTimeHelper::getCurrentDate()), QString("PDF-%1 (*.pdf)").arg(tr("Datei")));

  if (outputFile.isEmpty())
  {
    return;
  }

  QPrinter printer(QPrinter::HighResolution); //HighResolution
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPageSize(QPrinter::A4);
  int margin = 5;
  printer.setPageMargins(margin, margin, margin, margin, QPrinter::Millimeter);
  printer.setColorMode(QPrinter::GrayScale);
  printer.setOutputFileName(outputFile);

  ui->webView->page()->mainFrame()->print(&printer);

  if (ui->checkBox->isChecked())
  {
    // show pdf with external viewer
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir(outputFile).absolutePath()));
  }
}
