#include "Evaluation.h"
#include "ui_Evaluation.h"

#include <map>
#include <vector>
#include <algorithm>

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
#include <QString>
#include <QTextDocument>
#include <QFileDialog>

#include "ArticleManager.h"
#include "Settings.h"
#include "Statistics.h"
#include "EvaluationView.h"
#include "Seller.h"
#include "SellerManager.h"
#include "CssHelper.h"
#include "Converter.h"
#include "DateTimeHelper.h"
#include "ExcelExport.h"
#include "EvaluationByCategory.h"

Evaluation::Evaluation(ArticleManager* articleManager, Settings *settings, SellerManager *sellerManager, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Evaluation),
  m_articleManager(articleManager),
  m_settings(settings),
  m_sellerManager(sellerManager)
{
  ui->setupUi(this);

  m_statistics = new Statistics();

  m_overview = new EvaluationView(tr("Gesamtübersicht"), this);
  m_soldArticles = new EvaluationView(tr("Verkaufte Artikel"), this);
  m_unsoldArticles = new EvaluationView(tr("Nicht verkaufte Artikel"), this);

  ui->tabWidget->addTab(m_overview, m_overview->getName());
  ui->tabWidget->addTab(m_soldArticles, m_soldArticles->getName());
  ui->tabWidget->addTab(m_unsoldArticles, m_unsoldArticles->getName());

  m_excelExport = new ExcelExport(m_articleManager, m_settings, m_sellerManager, this);
  ui->tabWidget->addTab(m_excelExport, tr("Excel Export"));

  m_evaluationByCategory = new EvaluationByCategory(m_articleManager, this);
  ui->tabWidget->addTab(m_evaluationByCategory, tr("Auswertung nach Kategorie"));
}

Evaluation::~Evaluation()
{
  delete m_statistics;
  delete ui;
}

void Evaluation::doEvaluation()
{
  m_statistics->createStatistics(m_articleManager->getAllArticles(), m_settings->getDeductionPercentage());
  updateWebViews();
}

void Evaluation::updateWebViews()
{
  m_overview->setHtmlContent(createHtmlCodeOverview());
  m_soldArticles->setHtmlContent(createHtmlCodeSoldArticles());
  m_unsoldArticles->setHtmlContent(createHtmlCodeUnsoldArticles());
}

QString Evaluation::createHtmlCodeOverview()
{
  QString html;
  html.append("<!DOCTYPE html>");
  html.append("<html><head>");
  html.append("<meta charset=\"utf-8\">");
  html.append("<title></title>");
  html.append(CssHelper::createCssCode());
  html.append("</head><body>");

  html.append("<div class=\"page\">");
  html.append(QString("<h1>Zusammenfassung (erstellt am %1 um %2 Uhr)</h1>").arg(DateTimeHelper::getCurrentDateGerman()).arg(DateTimeHelper::getCurrentTimeGerman()));
  html.append("<h2>Allgemein</h2>");
  html.append("<table>");
  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("Umsatz")));
  html.append(QString("<td>%1</td>").arg(QString("%1 Euro").arg(QString::number(m_statistics->getVolumeOfSale(), 'f', 2).replace('.', ','))));
  html.append("</tr>");
  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("Einbehalt (%1 Prozent des Umsatzes)").arg(m_statistics->getDeductionPercentage())));
  html.append(QString("<td>%1</td>").arg(QString("%1 Euro").arg(QString::number(m_statistics->getDeduction(), 'f', 2).replace('.', ','))));
  html.append("</tr>");
  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("Anzahl der Verkäufe")));
  html.append(QString("<td>%1</td>").arg(QString("%1").arg(m_statistics->getQtySales())));
  html.append("</tr>");
  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("Anzahl der verkauften Artikel")));
  html.append(QString("<td>%1</td>").arg(QString("%1").arg(m_statistics->getQtySoldArticles())));
  html.append("</tr>");
  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("Artikel pro Verkauf")));
  html.append(QString("<td>%1</td>").arg(QString::number(m_statistics->getArticlesPerSale(), 'f', 1).replace('.', ',')));
  html.append("</tr>");
  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("Gesamtanzahl der Artikel")));
  html.append(QString("<td>%1</td>").arg(QString("%1").arg(m_statistics->getQtyAllArticles())));
  html.append("</tr>");

  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("- davon Kleidungsstücke")));
  html.append(QString("<td>%1 (%2)</td>").arg(m_statistics->getQtyClothes()).arg(QString("%1 %").arg(QString::number(m_statistics->getQtyClothes() * 100.0 / (m_statistics->getQtyAllArticles()), 'f', 1).replace('.', ','))));
  html.append("</tr>");

  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("- davon Nicht-Kleidungsstücke")));
  html.append(QString("<td>%1 (%2)</td>").arg(m_statistics->getQtyMisc()).arg(QString("%1 %").arg(QString::number(m_statistics->getQtyMisc() * 100.0 / (m_statistics->getQtyAllArticles()), 'f', 1).replace('.', ','))));
  html.append("</tr>");

  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("Verkaufte Artikel")));
  html.append(QString("<td>%1 (%2)</td>").arg(m_statistics->getQtySoldArticles()).arg(QString("%1 %").arg(QString::number(m_statistics->getPercentageOfSoldArticles(), 'f', 1).replace('.', ','))));
  html.append("</tr>");

  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("- davon Kleidungsstücke")));
  html.append(QString("<td>%1 (%2)</td>").arg(m_statistics->getQtySoldClothes()).arg(QString("%1 %").arg(QString::number(m_statistics->getQtySoldClothes() * 100.0 / m_statistics->getQtyAllArticles(), 'f', 1).replace('.', ','))));
  html.append("</tr>");

  html.append("<tr>");
  html.append(QString("<td>%1</td>").arg(tr("- davon Nicht-Kleidungsstücke")));
  html.append(QString("<td>%1 (%2)</td>").arg(m_statistics->getQtySoldMisc()).arg(QString("%1 %").arg(QString::number(m_statistics->getQtySoldMisc() * 100.0 / m_statistics->getQtyAllArticles(), 'f', 1).replace('.', ','))));
  html.append("</tr>");

  html.append("</table>");
  html.append("</div>");

  html.append("<div class=\"page\">");
  std::map<QString, int> offeredArticlesInRanges = m_articleManager->getOfferedArticlesInRanges();
  html.append("<h2>Anzahl der angebotenen Artikel</h2>");
  html.append("<table>");
  html.append("<tr>");
  html.append("<td>Angebotene Artikel</td>");
  html.append("<td>Anzahl Verkäufer</td>");
  html.append("</tr>");

  for (std::map<QString, int>::iterator it = offeredArticlesInRanges.begin(); it != offeredArticlesInRanges.end(); ++it)
  {
    html.append("<tr>");
    html.append(QString("<td>%1</td>").arg(it->first));
    html.append(QString("<td>%1</td>").arg(it->second));
    html.append("</tr>");
  }

  html.append("</table>");
  html.append("</div>");

  html.append("<div class=\"page\">");
  std::map<QString, int> soldArticlesInRanges = m_articleManager->getSoldArticlesInRanges();
  html.append("<h2>Anzahl der verkauften Artikel</h2>");
  html.append("<table>");
  html.append("<tr>");
  html.append("<td>Verkaufte Artikel</td>");
  html.append("<td>Anzahl Verkäufer</td>");
  html.append("</tr>");

  for (std::map<QString, int>::iterator it = soldArticlesInRanges.begin(); it != soldArticlesInRanges.end(); ++it)
  {
    html.append("<tr>");
    html.append(QString("<td>%1</td>").arg(it->first));
    html.append(QString("<td>%1</td>").arg(it->second));
    html.append("</tr>");
  }

  html.append("</table>");
  html.append("</div>");

  html.append("<div class=\"page\">");
  html.append("<h1>Umsatz pro Verkäufer</h1>");
  html.append("<table>");
  html.append("<tr>");
  html.append("<th>Verkäufernummer</th>");
  html.append("<th>Summe</th>");
  html.append("<th>Auszuzahlen</th>");
  html.append("</tr>");

  std::map<int, double> matrix = m_articleManager->getSalesPerSeller();
  double payOutFactor = m_articleManager->getPayOutFactor();

  int counter = 0;
  for (std::map<int, double>::iterator it = matrix.begin(); it != matrix.end(); ++it)
  {
    html.append("<tr>");
    html.append(QString("<td>%1</td>").arg(it->first));
    html.append(QString("<td>%1 Euro</td>").arg(QString::number(it->second, 'f', 2).replace('.', ',')));
    html.append(QString("<td>%1 Euro</td>").arg(QString::number(it->second * payOutFactor, 'f', 2).replace('.', ',')));
    html.append("</tr>");

    counter++;

    if (counter % 42 == 0)
    {
      html.append("</table>");
      html.append("</div>");
      html.append("<div class=\"page\">");
      html.append("<h1>Umsatz pro Verkäufer</h1>");
      html.append("<table>");
      html.append("<tr>");
      html.append("<th>Verkäufernummer</th>");
      html.append("<th>Summe</th>");
      html.append("<th>Auszuzahlen</th>");
      html.append("</tr>");
    }
  }

  html.append("</table>");
  html.append("</div>");

  html.append("</body></html>");

  return html;
}

QString Evaluation::createHtmlCodeSoldArticles()
{
  // unfortunately there is a bug in webkit and page-break is not supported!
  // as a hack we use div class page with fixed height
  // do not use h1 (or other stuff with margin or padding)!
  // use tested elements like <b> <br /> <table>

  QString html;
  html.append("<!DOCTYPE html>");
  html.append("<html><head>");
  html.append("<title></title>");
  html.append(CssHelper::createCssCode());
  html.append("</head><body>");

  std::map<int, double> matrix = m_articleManager->getSalesPerSeller();
  double payOutFactor = m_articleManager->getPayOutFactor();

  for (std::map<int, double>::iterator it = matrix.begin(); it != matrix.end(); ++it)
  {
    std::map<int, double> articles = m_articleManager->getSoldArticles(it->first);

    Seller* seller = m_sellerManager->getSeller(it->first);

    if (seller == 0)
    {
      continue;
    }

    html.append("<div class=\"page\">");
    /*if (seller != 0)
    {
      html.append(QString("<h1>Verkäufernummer: %1   Name: %2 %3   Telefon: %4</h1>").arg(it->first).arg(seller->m_firstName).arg(seller->m_lastName).arg(seller->m_phone));
    }
    else
    {
      html.append(QString("<h1>Verkäufernummer: %1</h1>").arg(it->first));
    }*/

    html.append("<table>");
    html.append("<tr>");
    html.append(QString("<td>Verkäufernummer</td>"));
    html.append(QString("<td>%1</td>").arg(it->first));
    html.append("</tr>");
    html.append("<tr>");
    html.append(QString("<td>Name</td>"));

    QString sellerName;

    if (seller != 0)
    {
      sellerName = seller->m_firstName + " " + seller->m_lastName;
    }

    html.append(QString("<td>%1</td>").arg(sellerName));
    html.append("</tr>");
    html.append("<tr>");
    html.append(QString("<td>Telefon</td>"));
    if (seller != 0)
    {
      html.append(QString("<td>%1</td>").arg(seller->m_phone));
    }
    else
    {
      html.append(QString("<td></td>"));
    }
    html.append("</tr>");
    html.append("<tr>");
    html.append(QString("<td>Umsatz</td>"));
    html.append(QString("<td>%1 Euro</td>").arg(QString::number(it->second, 'f', 2).replace('.', ',')));
    html.append("</tr>");
    html.append("<tr>");
    html.append(QString("<td>Einbehalt</td>"));
    html.append(QString("<td>%1 Euro</td>").arg(QString::number(it->second * (1.0 - payOutFactor), 'f', 2).replace('.', ',')));
    html.append("</tr>");
    html.append("<tr>");
    html.append(QString("<td><b>Auszahlbetrag</b></td>"));
    html.append(QString("<td><b>%1 Euro</b></td>").arg(QString::number(it->second * payOutFactor, 'f', 2).replace('.', ',')));
    html.append("</tr>");
    html.append("</table>");

    if (articles.empty())
    {
      html.append("</div>");
      continue;
    }

    bool columnDiscrepancyNeeded = false;
    for (std::map<int, double>::iterator itA = articles.begin(); itA != articles.end(); ++itA)
    {
      Article* article = m_articleManager->getArticle(it->first, itA->first);
      if (article->m_prize != article->m_listPrize)
      {
        columnDiscrepancyNeeded = true;
      }
    }

    //html.append("<h2>Liste der verkauften Artikel</h2>");
    html.append("<br />");
    html.append("<table>");
    appendSoldArticleHeader(html, columnDiscrepancyNeeded);

    int counter = 0;
    for (std::map<int, double>::iterator itA = articles.begin(); itA != articles.end(); ++itA)
    {
      if (counter != 0 && ((counter - 37) % 42 == 0)) // %44
      {
        html.append("</table>");
        html.append("</div>");
        html.append("<div class=\"page\">");
        html.append(QString("<h2>Liste der verkauften Artikel von Verkäufer %1 - %2</h2>").arg(it->first).arg(sellerName));
        html.append("<table>");
        appendSoldArticleHeader(html, columnDiscrepancyNeeded);
      }

      Article* article = m_articleManager->getArticle(it->first, itA->first);

      html.append("<tr>");
      html.append(QString("<td>%1</td>").arg(itA->first));
      //html.append(QString("<td>%1 Euro</td>").arg(QString::number(itA->second, 'f', 2).replace('.', ',')));
      html += QString("<td class=\"prize\">%1 &euro;</td>").arg(Converter::prizeToString(article->m_prize));

      html.append(QString("<td>%1</td>").arg(article->m_size));
      html.append(QString("<td>%1</td>").arg(article->m_description));

      if (columnDiscrepancyNeeded)
      {
        if (article->m_prize != article->m_listPrize)
        {
          if (article->m_listPrize > 0.001)
          {
            html += QString("<td>Listenpreis: %1 &euro;</td>").arg(Converter::prizeToString(article->m_listPrize));
          }
          else
          {
            html += QString("<td>Artikel wurde nicht online eingegeben.</td>");
          }
        }
        else
        {
          html += "<td></td>";
        }
      }


      //html += QString("<td>%1</td>").arg(article->m_soldOnPc);



      html.append("</tr>");
      counter++;
    }

    html.append("</table>");
    html.append("</div>");
  }

  html.append("</body></html>");

  return html;
}

QString Evaluation::createHtmlCodeUnsoldArticles()
{
  QString html;
  html.append("<!DOCTYPE html>");
  html.append("<html><head>");
  html.append("<meta charset=\"utf-8\">");
  html.append("<title></title>");
  html.append(CssHelper::createCssCode(false, true));
  html.append("</head><body>");

  std::map<int, double> matrix = m_articleManager->getSalesPerSeller();

  for (std::map<int, double>::iterator it = matrix.begin(); it != matrix.end(); ++it)
  {
    std::vector<int> unsoldArticles = m_articleManager->getUnsoldArticles(it->first);

    if (unsoldArticles.empty())
    {
      continue;
    }

    html.append("<div class=\"page\">");
    html.append(QString("<b>Nicht verkaufte Artikel von Verkäufer %1</b><br /><br />").arg(it->first));
    html.append("<table>");

    int counter = 0;
    const int columnCount = 10;
    for (int i = m_settings->getArticleMin(); i <= m_settings->getArticleMax(); i++)
    {
      if (counter % columnCount == 0)
      {
        html.append("<tr>");
      }

      //html.append(QString("<td>%1</td>").arg(itA->first));

      if (std::find(unsoldArticles.begin(), unsoldArticles.end(), i) != unsoldArticles.end())
      {
        html.append(QString("<td>%1</td>").arg(i));
      }
      else
      {
        html.append(QString("<td>&nbsp;&nbsp;&nbsp;</td>"));
      }

      counter++;

      if (counter % columnCount == 0)
      {
        html.append("</tr>");
      }
    }

    /*int counter = 0;
    const int columnCount = 5;
    for (auto itA = unsoldArticles.begin(); itA != unsoldArticles.end(); ++itA)
    {
        if (counter % columnCount == 0)
        {
            html.append("<tr>");
        }

        html.append(QString("<td>%1</td>").arg(itA->first));
        counter++;

        if (counter % columnCount == 0)
        {
            html.append("</tr>");
        }
    }

    while (counter % columnCount != 0)
    {
        html.append(QString("<td></td>"));
        counter++;

        if (counter % columnCount == 0)
        {
            html.append("</tr>");
        }
    }*/

    html.append("</table>");
    html.append("</div>");
  }

  html.append("</body></html>");

  return html;
}

void Evaluation::appendSoldArticleHeader(QString &html, bool columnDiscrepancyNeeded)
{
  html.append("<tr>");
  html.append("<th>Artikelnummer</th>");
  html.append("<th>Preis</th>");
  html.append("<th>Größe</th>");
  html.append("<th>Artikelbeschreibung</th>");
  if (columnDiscrepancyNeeded)
  {
    html.append("<th>Abweichungen</th>");
  }
  //html.append("<th>Kasse</th>");
  html.append("</tr>");
}
