#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>
#include <QRegExp>
#include <QRegExpValidator>

#if QT_VERSION >= 0x050000
#include <QtWebKitWidgets/QWebFrame>
#else
#include <QWebFrame>
#endif

#include "TuKiBasar.h"
#include "ui_TuKiBasar.h"

#include "Article.h"
#include "ArticleManager.h"
#include "Evaluation.h"
#include "Settings.h"
#include "PrizeCorrection.h"
#include "Seller.h"
#include "SellerManager.h"
#include "Converter.h"
#include "SalesView.h"
#include "PasswordInput.h"
#include "ArticleReturn.h"

TuKiBasar::TuKiBasar(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TuKiBasar)
{
  ui->setupUi(this);

  QRegExp rx ("[0-9]{6}");
  ui->lineEditInput->setValidator(new QRegExpValidator (rx, this));

  m_settings = new Settings();
  m_sellerManager = new SellerManager();
  m_articleManager = new ArticleManager(m_settings, "Articles.xml");
  m_evaluation = new Evaluation(m_articleManager, m_settings, m_sellerManager);

  prepareForNextInput();

  setCashPointNumber();

  ui->plainTextEditArticleList->setVisible(false); // TODO that GUI-element should be removed, but there is a problem with the QCloseEvent, when this element is removed in the UI-File

  m_passwortProtectedActions.append(ui->actionArticleReturn);
  m_passwortProtectedActions.append(ui->actionImportArticleLists);
  m_passwortProtectedActions.append(ui->actionExportSoldArticles);
  m_passwortProtectedActions.append(ui->actionEvaluation);
  m_passwortProtectedActions.append(ui->actionSettings);
  m_passwortProtectedActions.append(ui->actionCompleteEvaluation);
  m_passwortProtectedActions.append(ui->actionDeactivateAdvancedAccess);

  #ifndef Q_OS_MAC
    // on Mac OS (development enviroment) we do not need password protection
    setPasswordProtectedActionsVisible(false);
  #endif

  // hide description since there have been big layout issues on PCs with Aspect Ratio 4:3
  ui->labelDescription->setVisible(false);
  ui->labelDescriptionCaption->setVisible(false);

  updateInformation();
}

TuKiBasar::~TuKiBasar()
{
  delete m_evaluation;
  delete m_articleManager;
  delete m_sellerManager;
  delete m_settings;
  delete ui;
}

void TuKiBasar::on_actionArticleReturn_triggered()
{
  ArticleReturn articleReturn;
  if (articleReturn.exec() == QDialog::Accepted)
  {
    bool returnedSuccesfully = m_articleManager->returnArticle(articleReturn.getSellerNumber(), articleReturn.getArticleNumber());
    QString message;
    if (returnedSuccesfully)
    {
      message = tr("Artikel wurde erfolgreich zurückgegben.");
      m_articleManager->toXml();
    }
    else
    {
      message = tr("Artikel konnte nicht zurückgegben werden.");
    }
    QMessageBox messageBox;
    messageBox.setText(message);
    messageBox.exec();
  }
}

void TuKiBasar::on_actionSettings_triggered()
{
  /*if (!checkPassword())
    {
        return;
    }*/
  m_settings->exec();
  setCashPointNumber();
}

void TuKiBasar::on_actionEvaluation_triggered()
{
  /*if (!checkPassword())
    {
        return;
    }*/

  askUserToFinishCurrentSale();

  m_evaluation->doEvaluation();
  m_evaluation->exec();
}

void TuKiBasar::on_actionImportArticleLists_triggered()
{
  /*if (!checkPassword())
    {
        return;
    }*/

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, tr("Vorhandene Artikel löschen?"),
                                tr("Möchten Sie die bereits importierten Artikel löschen?"),
                                QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes)
  {
    m_articleManager->clear();
    m_sellerManager->clear();
  }

  QString dirName = QFileDialog::getExistingDirectory(this, tr("Bitte Ordner mit den Artikellisten wählen...")); //TODO set folder of last selection?

  if (dirName.isEmpty())
  {
    return;
  }

  QDir dir(dirName);

  QStringList filters;
  filters << "articleList_*.txt";
  dir.setNameFilters(filters);

  QStringList fileNames = dir.entryList();

  unsigned int sellerCounter = 0;
  unsigned int articleCounter = 0;

  const int headerOffset = 6;
  const int linesPerArticle = 4;

  for (int i = 0; i < fileNames.length(); i++)
  {
    QString filePath = dir.filePath(fileNames.at(i));

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      continue;
    }

    QStringList fileContent;
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd())
    {
      fileContent.append(in.readLine());
    }

    if (fileContent.length() < headerOffset)
    {
      file.close();
      continue;
    }

    if (fileContent.at(0) != "Article List")
    {
      file.close();
      continue;
    }

    if ((fileContent.length() - headerOffset) % linesPerArticle != 0)
    {
      file.close();
      continue;
    }

    //TODO use version information if nessessary

    bool conversionSellerNumber;
    int sellerNumber = fileContent.at(2).toInt(&conversionSellerNumber);

    if (!conversionSellerNumber)
    {
      file.close();
      continue;
    }

    if (sellerNumber < m_settings->getSellerMin() || sellerNumber > m_settings->getSellerMax())
    {
      continue;
    }

    //qDebug() << fileContent.at(3) << fileContent.at(4);

    Seller* seller = new Seller(sellerNumber, fileContent.at(3), fileContent.at(4), fileContent.at(5));
    m_sellerManager->addSeller(seller);

    int numberOfArticles = (fileContent.length() - headerOffset) / linesPerArticle;

    for (int i = 0; i < numberOfArticles; i++)
    {
      bool conversionPrize;
      double prize = fileContent.value(headerOffset + linesPerArticle * i + 1).replace(",", ".").toDouble(&conversionPrize);
      if (!conversionPrize)
      {
        //continue;
        prize = 0.0;
      }

      int articleNumber = fileContent.at(headerOffset + linesPerArticle * i).toInt();

      if (articleNumber < m_settings->getArticleMin() || articleNumber > m_settings->getArticleMax())
      {
        continue;
      }

      QString size = fileContent.at(headerOffset + linesPerArticle * i + 2);
      QString description = fileContent.at(headerOffset + linesPerArticle * i + 3);

      //qDebug() << sellerNumber << articleNumber << description;

      if (prize < 0.01 && description.isEmpty())
      {
        continue;
      }

      Article* article = new Article(articleNumber, sellerNumber, 0, prize, prize, size, description, "");
      m_articleManager->addArticle(article); //TODO check that no article is added twice

      articleCounter++;
    }

    file.close();
    sellerCounter++;
  }

  m_articleManager->toXml();

  QMessageBox mb;
  if (articleCounter > 0)
  {
    mb.setText(tr("Es wurden erfolgreich %1 Artikel von %2 Verkäufern importiert.").arg(articleCounter).arg(sellerCounter));
  }
  else
  {
    mb.setText(tr("Es wurden keine Artikel importiert."));
  }

  mb.exec();
}

void TuKiBasar::on_lineEditInput_returnPressed()
{
  QString input = ui->lineEditInput->text();

  bool conversion1 = false;
  bool conversion2 = false;

  int sellerNumber = input.left(3).toInt(&conversion1);
  int articleNumber = input.right(3).toInt(&conversion2);

  if (input.left(3) == ui->labelSellerNumber->text() && input.right(3) == ui->labelArticleNumber->text())
  {
    ui->lineEditInput->clear();
    return;
  }

  if (!conversion1 || !conversion2)
  {
    // should not happen, since we use a QRegExpValidator
    QMessageBox mb;
    mb.setText(tr("Die Eingabe ist fehlerhaft!"));
    mb.exec();
    ui->lineEditInput->selectAll();
    return;
  }

  if (sellerNumber > m_settings->getSellerMax())
  {
    QMessageBox mb;
    mb.setText(tr("Die eingegebene Verkäufernummer ist zu hoch!\nDas Maximum ist %1!").arg(m_settings->getSellerMax()));
    mb.exec();
    ui->lineEditInput->selectAll();
    return;
  }

  if (sellerNumber < m_settings->getSellerMin())
  {
    QMessageBox mb;
    mb.setText(tr("Die eingegebene Verkäufernummer ist zu niedrig!\nDas Minimum ist %1!").arg(m_settings->getSellerMin()));
    mb.exec();
    ui->lineEditInput->selectAll();
    return;
  }

  if (articleNumber > m_settings->getArticleMax())
  {
    QMessageBox mb;
    mb.setText(tr("Die eingegebene Artikelnummer ist zu hoch!\nDas Maximum ist %1!").arg(m_settings->getArticleMax()));
    mb.exec();
    ui->lineEditInput->selectAll();
    return;
  }

  if (articleNumber < m_settings->getArticleMin())
  {
    QMessageBox mb;
    mb.setText(tr("Die eingegebene Artikelnummer ist zu niedrig!\nDas Minimum ist %1!").arg(m_settings->getArticleMin()));
    mb.exec();
    ui->lineEditInput->selectAll();
    return;
  }

  if (m_articleManager->isArticleInCurrentSale(sellerNumber, articleNumber))
  {
    QMessageBox mb;
    mb.setText(tr("Der eingegebene Artikel ist bereits in der aktuellen Artikelliste enthalten!"));
    mb.exec();
    ui->lineEditInput->selectAll();
    return;
  }

  Article* article = m_articleManager->getArticle(sellerNumber, articleNumber);

  bool prizeCorrectionRequired = false;
  if (article != 0)
  {
    if (article->m_soldOnPc != 0)
    {
      QMessageBox mb;
      mb.setText(tr("Der eingegebene Artikel wurde bereits verkauft!"));
      mb.exec();
      ui->lineEditInput->selectAll();
      return;
    }

    if (article->m_prize < 0.01)
    {
      QMessageBox mb;
      mb.setText(tr("Für den eingegebene Artikel ist im System kein Preis hinterlegt.\nBitte Preis manuell eingeben!"));
      mb.exec();
      prizeCorrectionRequired = true;
    }

    m_articleManager->addArticleToCurrentSale(article);
  }
  else
  {
    Article* newArticle = new Article(articleNumber, sellerNumber, 0, 0.0, 0.0, "", "", "");
    m_articleManager->addArticle(newArticle);
    m_articleManager->addArticleToCurrentSale(newArticle);

    QMessageBox mb;
    mb.setText(tr("Der eingegebene Artikel ist im System nicht hinterlegt.\nBitte Preis manuell eingeben!"));
    mb.exec();
    prizeCorrectionRequired = true;
  }

  setLastArticleInformation();
  updateArticleView();

  ui->lineEditInput->clear();

  if (prizeCorrectionRequired)
  {
    on_pushButtonCorrectPrize_clicked();
  }
}

void TuKiBasar::on_pushButtonDeleteLastInput_clicked()
{
  m_articleManager->removeLastArticleFromCurrentSale();
  Article* article = m_articleManager->getLastArticleInCurrentSale();

  if (article != 0)
  {
    setLastArticleInformation();
  }
  else
  {
    clearLastArticleInformation();
  }

  updateArticleView();
  prepareForNextInput();
}

void TuKiBasar::setLastArticleInformation()
{
  Article *article = m_articleManager->getLastArticleInCurrentSale();

  ui->labelArticleNumber->setText(QString("%1").arg(article->m_articleNumber));
  ui->labelSellerNumber->setText(QString("%1").arg(article->m_sellerNumber));
  ui->labelDescription->setText(article->m_description);
  ui->labelPrize->setText(Converter::prizeToString(article->m_prize, false));
}

void TuKiBasar::clearLastArticleInformation()
{
  ui->labelArticleNumber->clear();
  ui->labelSellerNumber->clear();
  ui->labelDescription->clear();
  ui->labelPrize->clear();
}

void TuKiBasar::updateArticleView()
{
  //ui->plainTextEditArticleList->setPlainText(m_articleManager->currentSaleToText());
  ui->webViewArticleList->setHtml(m_articleManager->currentSaleToHtml());
  //ui->webViewArticleList->triggerPageAction(QWebPage::MoveToEndOfDocument);
  ui->webViewArticleList->page()->mainFrame()->setScrollBarValue(Qt::Vertical, ui->webViewArticleList->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
  ui->labelSum->setText(Converter::prizeToString(m_articleManager->getSumOfCurrentSale()));
  calculateChange();
}

void TuKiBasar::askUserToFinishCurrentSale()
{
  if(m_articleManager->isCurrentSaleEmpty())
  {
    return;
  }

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, tr("Aktuellen Verkauf abschließen?"),
                                tr("Möchten Sie erst den aktuellen Verkauf abschließen?"),
                                QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    on_pushButtonNextCustomer_clicked();
  }
}

void TuKiBasar::prepareForNextInput()
{
  ui->lineEditInput->clear();
  ui->lineEditInput->setFocus();
}

bool TuKiBasar::checkPassword()
{
  PasswordInput passwordInput;

  if (passwordInput.exec() != QDialog::Accepted)
  {
    return false;
  }

  if (passwordInput.getPassword() != "TuKiTante")
  {
    QMessageBox mb;
    mb.setText(tr("Falsches Passwort!"));
    mb.exec();
    return false;
  }

  return true;
}

void TuKiBasar::calculateChange()
{
  double moneyGiven = ui->doubleSpinBoxMoneyGiven->value();
  double sum = m_articleManager->getSumOfCurrentSale();
  ui->labelChange->setText(Converter::prizeToString(moneyGiven - sum));
}

void TuKiBasar::setCashPointNumber()
{
  ui->labelCashPointNumber->setText(tr("Kasse %1 ").arg(m_settings->getPc()));
}

void TuKiBasar::updateInformation()
{
  //ui->statusBar->showMessage(tr("Anzahl der Verkäufe: %1   Anzahl der verkauften Artikel: %2").arg(m_articleManager->getCountOfTransactions()).arg(m_articleManager->getCountOfSoldArticles()));
  ui->labelCountSoldArticles->setText(QString::number(m_articleManager->getCountOfSoldArticles()));
  ui->labelCountTransactions->setText(QString::number(m_articleManager->getCountOfTransactions()));
}

void TuKiBasar::setPasswordProtectedActionsVisible(bool visible)
{
  for (QList<QAction*>::iterator it = m_passwortProtectedActions.begin(); it != m_passwortProtectedActions.end(); it++)
  {
    (*it)->setVisible(visible);
  }
}

void TuKiBasar::on_pushButtonNextCustomer_clicked()
{    
  if (m_articleManager->isCurrentSaleEmpty())
  {
    ui->doubleSpinBoxMoneyGiven->setValue(0.0);
    prepareForNextInput();
    return;
  }

  m_articleManager->finishCurrentSale(m_settings->getPc());
  ui->doubleSpinBoxMoneyGiven->setValue(0.0);
  clearLastArticleInformation();
  updateArticleView();
  prepareForNextInput();
  updateInformation();
}

void TuKiBasar::on_actionCompleteEvaluation_triggered()
{
  /*if (!checkPassword())
    {
        return;
    }*/

  askUserToFinishCurrentSale();

  QStringList files = QFileDialog::getOpenFileNames(this, "Bitte Dateien auswählen", "", "XML-Dateien (*.xml)");

  if (files.empty())
  {
    return;
  }

  ArticleManager* totalArticleManager = new ArticleManager(m_settings, files.at(0));

  for (int i = 1; i < files.length(); i++)
  {
    ArticleManager* articleManagerToSync = new ArticleManager(m_settings, files.at(i));
    totalArticleManager->sync(articleManagerToSync);
    delete articleManagerToSync;
  }

  Evaluation* totalEvaluation = new Evaluation(totalArticleManager, m_settings, m_sellerManager);
  totalEvaluation->doEvaluation();
  totalEvaluation->exec();

  delete totalEvaluation;
  delete totalArticleManager;

  prepareForNextInput();
}

void TuKiBasar::on_pushButtonCorrectPrize_clicked()
{
  Article* article = m_articleManager->getLastArticleInCurrentSale();

  if (article == 0)
  {
    prepareForNextInput();
    return;
  }

  PrizeCorrection prizeCorrection;
  prizeCorrection.setPrize(article->m_prize);
  if (prizeCorrection.exec() == QDialog::Accepted)
  {
    article->m_prize = prizeCorrection.getPrize();
  }

  updateArticleView();
  setLastArticleInformation();
  prepareForNextInput();
}

void TuKiBasar::closeEvent(QCloseEvent *event)
{
  askUserToFinishCurrentSale();

  event->accept();
}

void TuKiBasar::on_actionExportSoldArticles_triggered()
{
  /*if (!checkPassword())
    {
        return;
    }*/

  askUserToFinishCurrentSale();

  QString outputFile = QFileDialog::getSaveFileName(this, tr("Bitte geben Sie den Dateinamen an"), QString("%1_PC%2.xml").arg(tr("VerkaufteArtikel")).arg(m_settings->getPc()), QString("XML-%1 (*.xml)").arg(tr("Datei")));

  if (outputFile.isEmpty())
  {
    return;
  }

  QString toRestore = m_articleManager->getFileName();

  m_articleManager->setFileName(outputFile);
  m_articleManager->toXml();

  m_articleManager->setFileName(toRestore);

  QMessageBox messageBox;
  messageBox.setText(tr("Verkaufte Artikel wurden erfolgreich exportiert!"));
  messageBox.exec();
}

void TuKiBasar::on_doubleSpinBoxMoneyGiven_valueChanged(double moneyGiven)
{
  calculateChange();
}

void TuKiBasar::on_actionActivateAdvancedAccess_triggered()
{
  if (checkPassword())
  {
    setPasswordProtectedActionsVisible(true);
    ui->actionActivateAdvancedAccess->setVisible(false);
  }
}

void TuKiBasar::on_actionDeactivateAdvancedAccess_triggered()
{
  setPasswordProtectedActionsVisible(false);
  ui->actionActivateAdvancedAccess->setVisible(true);
}

void TuKiBasar::on_pushButtonSalesHistorie_clicked()
{
  SalesView salesView;
  salesView.setTransactions(m_articleManager->getTransactions());
  salesView.exec();
}
