#include <QClipboard>
#include <QMessageBox>

#include "ExcelExport.h"
#include "ui_ExcelExport.h"

#include "ArticleManager.h"
#include "Settings.h"
#include "SellerManager.h"
#include "Seller.h"

ExcelExport::ExcelExport(ArticleManager* articleManager, Settings* settings, SellerManager* sellerManager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExcelExport),
    m_articleManager(articleManager),
    m_sellerManager(sellerManager),
    m_settings(settings)
{
    ui->setupUi(this);
}

ExcelExport::~ExcelExport()
{
    delete ui;
}

void ExcelExport::on_pushButtonExportSalesPerSeller_clicked()
{
    QString excelExport;

    excelExport.append("Verkäufernummer\tVorname\tNachname\tUmsatz\tAuszuzahlen\n");

    std::map<int, double> matrix = m_articleManager->getSalesPerSeller();
    double payOutFactor = m_articleManager->getPayOutFactor();

    int counter = 0;
    for (std::map<int, double>::iterator it = matrix.begin(); it != matrix.end(); ++it)
    {
        Seller* seller = m_sellerManager->getSeller(it->first);

        QString firstName;
        QString lastName;
        if (seller)
        {
            firstName = seller->m_firstName;
            lastName = seller->m_lastName;
        }

        excelExport.append(QString("%1\t").arg(it->first));
        excelExport.append(QString("%1\t").arg(firstName));
        excelExport.append(QString("%1\t").arg(lastName));
        excelExport.append(QString("%1\t").arg(QString::number(it->second, 'f', 2).replace('.', ',')));
        excelExport.append(QString("%1\n").arg(QString::number(it->second * payOutFactor, 'f', 2).replace('.', ',')));

        /*html.append("<tr>");
        html.append(QString("<td>%1</td>").arg(it->first));
        html.append(QString("<td>%1 Euro</td>").arg(QString::number(it->second, 'f', 2).replace('.', ',')));
        html.append(QString("<td>%1 Euro</td>").arg(QString::number(it->second * payOutFactor, 'f', 2).replace('.', ',')));
        html.append("</tr>");*/
    }

    QApplication::clipboard()->setText(excelExport);

    QMessageBox mb;
    mb.setText("Informationen wurden in die Zwischenablage kopiert und können in Excel kopiert werden!");
    mb.exec();
}
