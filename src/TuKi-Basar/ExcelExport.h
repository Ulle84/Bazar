#ifndef EXCELEXPORT_H
#define EXCELEXPORT_H

#include <QWidget>

class ArticleManager;
class Settings;
class SellerManager;

namespace Ui {
class ExcelExport;
}

class ExcelExport : public QWidget
{
    Q_OBJECT

public:
    explicit ExcelExport(ArticleManager* articleManager, Settings* settings, SellerManager* sellerManager, QWidget *parent = 0);
    ~ExcelExport();

private slots:
    void on_pushButtonExportSalesPerSeller_clicked();

private:
    Ui::ExcelExport *ui;
    ArticleManager* m_articleManager;
    SellerManager* m_sellerManager;
    Settings* m_settings;
};

#endif // EXCELEXPORT_H
