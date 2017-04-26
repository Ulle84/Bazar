#include "SalesView.h"
#include "ui_SalesView.h"
#include "Article.h"
#include "CssHelper.h"
#include "Converter.h"

SalesView::SalesView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesView),
    m_index(0)
{
    ui->setupUi(this);
}

SalesView::~SalesView()
{
    delete ui;
}

void SalesView::setTransactions(QMap<QString, QList<Article *> > transactions)
{
    m_transactions = transactions;
    m_index = transactions.size() - 1;
    update();
}

void SalesView::on_pushButtonPrevious_clicked()
{
    m_index--;
    update();
}

void SalesView::on_pushButtonNext_clicked()
{
    m_index++;
    update();
}

void SalesView::update()
{
    if (m_transactions.size() == 0)
    {
        ui->labelIndex->setText(QString("0 / 0"));
        ui->pushButtonPrevious->setEnabled(false);
        ui->pushButtonNext->setEnabled(false);
    }
    else
    {
        ui->labelIndex->setText(QString("%1 / %2").arg(m_index + 1).arg(m_transactions.size()));

        if (m_index == m_transactions.size() - 1)
        {
            ui->pushButtonNext->setEnabled(false);
        }
        else
        {
            ui->pushButtonNext->setEnabled(true);
        }

        if (m_index == 0)
        {
            ui->pushButtonPrevious->setEnabled(false);
        }
        else
        {
            ui->pushButtonPrevious->setEnabled(true);
        }

        setContent(m_index);
    }
}

void SalesView::setContent(int index)
{
    QString currentSaleHtml;
    currentSaleHtml += "<!DOCTYPE html>";
    currentSaleHtml += "<meta charset=\"utf-8\">";
    currentSaleHtml += "<html>";
    currentSaleHtml += CssHelper::createCssCode(true);
    currentSaleHtml += "<table>";
    currentSaleHtml += "<tr>";
    currentSaleHtml += "<th>Verkäufer</th>";
    currentSaleHtml += "<th>Artikel</th>";
    currentSaleHtml += "<th>Preis</th>";
    currentSaleHtml += "<th>Beschreibung</th>";
    currentSaleHtml += "</tr>";

    int counter = -1;
    double sum = 0.0;
    for (QMap<QString, QList<Article*> >::iterator it= m_transactions.begin(); it != m_transactions.end(); it++)
    {
        counter++;
        if (counter != index)
        {
            continue;
        }

        ui->labelDateTime->setText(it.key());

        QList<Article*> currentSale = it.value();

        for (QList<Article*>::iterator it = currentSale.begin(); it != currentSale.end(); ++it)
        {
            currentSaleHtml += "<tr>";
            currentSaleHtml += QString("<td>%1</td>").arg((*it)->m_sellerNumber);
            currentSaleHtml += QString("<td>%1</td>").arg((*it)->m_articleNumber);
            currentSaleHtml += QString("<td class=\"prize\">%1 &euro;</td>").arg(Converter::prizeToString((*it)->m_prize));
            currentSaleHtml += QString("<td>%1</td>").arg((*it)->m_description);
            currentSaleHtml += "</tr>";

            sum += (*it)->m_prize;
        }
        break;
    }

    currentSaleHtml += "</table>";

    currentSaleHtml += QString("<br /><b>Summe: %1 &euro;</b>").arg(Converter::prizeToString(sum));

    currentSaleHtml += "</html>";

    ui->webViewSale->setHtml(currentSaleHtml);
}

void SalesView::on_pushButtonOk_clicked()
{
    this->accept();
}
