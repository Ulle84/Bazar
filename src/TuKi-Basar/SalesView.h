#ifndef SALESVIEW_H
#define SALESVIEW_H

#include <QDialog>
#include <QMap>
#include <QString>
#include <QList>

class Article;

namespace Ui {
class SalesView;
}

class SalesView : public QDialog
{
    Q_OBJECT

public:
    explicit SalesView(QWidget *parent = 0);
    ~SalesView();

    void setTransactions(QMap<QString, QList<Article*> > transactions);

private slots:
    void on_pushButtonPrevious_clicked();
    void on_pushButtonNext_clicked();
    void on_pushButtonOk_clicked();

private:
    void update();
    void setContent(int index);

    Ui::SalesView *ui;
    int m_index;
    QMap<QString, QList<Article*> > m_transactions;
};

#endif // SALESVIEW_H
