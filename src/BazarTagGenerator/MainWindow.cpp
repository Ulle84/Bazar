#include <QDebug>
#include <QDateTime>
#include <QPrinter>
#include <QWebFrame>
#include <QMessageBox>
#include <QDomDocument>
#include <QString>
#include <QStringList>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "BarCodeGenerator.h"
#include "ConversionHelper.h"

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_fileName("Settings.xml")
{
  ui->setupUi(this);
  ui->progressBar->setValue(0);

  fromXml();
}

MainWindow::~MainWindow()
{
  toXml();
  delete ui;
}

void MainWindow::on_webView_loadFinished(bool loadFinished)
{
  if (!loadFinished)
  {
    return;
  }

  QString outputFile = "output.pdf";

  QPrinter printer(QPrinter::HighResolution); //HighResolution
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPageSize(QPrinter::A4);
  int margin = 0;
  printer.setPageMargins(margin, margin, margin, margin, QPrinter::Millimeter);
  printer.setColorMode(QPrinter::GrayScale);
  printer.setOutputFileName(outputFile);
  ui->webView->page()->mainFrame()->print(&printer);


  // show pdf with external viewer
  QDesktopServices::openUrl(QUrl::fromLocalFile(QDir(outputFile).absolutePath()));
}

void MainWindow::on_pushButtonStart_clicked()
{
  QString fontFamily = "Calibri, Helvetica, Times New Roman, Verdana";

  if (ui->spinBoxProductMax->value() < ui->spinBoxProductMin->value())
  {
   QMessageBox messageBox;
   messageBox.setWindowTitle(tr("Falsche Eingabe"));
   messageBox.setText(tr("Artikelnummern sind falsch eingegeben!"));
   messageBox.exec();
  }
  //TODO setMin and max

  ui->pushButtonStart->setEnabled(false);

  BarCodeGenerator barCodeGenerator;

  QByteArray output;

  QByteArray html;
  html.append("<html><head><title>test</title></head><body>");

  ui->progressBar->setValue(0);
  const int columns = 4;
  const int lines = 10;
  const int widthA4Millimeter = 210;
  const int heightA4Millimeter = 297;
  const int totalHeight = 1402;
  const int totalWidth = 991; //991.3131
  const int globalMarginLeft = 38;
  const int globalMarginTop = 99;
  const int marginLeft = 10;
  const int marginRight = marginLeft;
  const int marginTop = 10;
  const int marginBottom = marginTop;
  const double width = 228.75;
  const double height = 120.4;

  // 1px -> 297  / 1402 -> 0,21184022824536376604850213980029 mm
  // 1mm -> 1402 /  297 -> 4,7205387205387205387205387205387 px

  int numSellers  = ui->spinBoxSellerMax->value()  - ui->spinBoxProductMin->value() + 1;
  int numProducts = ui->spinBoxProductMax->value() - ui->spinBoxProductMin->value() + 1;
  int numberOfCodes = numSellers * numProducts;
  const int fontSize = 20;

  const int svgWidth = 208; //TODO calculate
  const int svgHeight = 100;

  int index = 0;
  int progress = 0;
  for (int i = ui->spinBoxSellerMin->value(); i <= ui->spinBoxSellerMax->value(); i++)
  {
    for (int j = ui->spinBoxProductMin->value(); j <= ui->spinBoxProductMax->value(); j++)
    {
      barCodeGenerator.generateSvg(ConversionHelper::numbersToString(i, j), output);
      //barCodeGenerator.generateSvg("1,50", output);

      QDomDocument domDocument;
      QString errorString;
      int errorLine;
      int errorColumn;

      if (!domDocument.setContent(output, true, &errorString, &errorLine, &errorColumn))
      {
          QMessageBox::information(window(), tr("Error"), tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorString));
          return;
      }

      QDomElement svg = domDocument.documentElement();
      QDomElement addHere = svg.elementsByTagName("g").at(0).toElement();
      svg.setAttribute("width", svgWidth);
      svg.setAttribute("height", svgHeight);

      QDomNodeList rects = svg.elementsByTagName("rect");
      for (unsigned int k = 0; k < rects.length(); k++)
      {
        double x = rects.at(k).toElement().attribute("x").toDouble();

        rects.at(k).toElement().setAttribute("y", 67);
        rects.at(k).toElement().setAttribute("x", QString::number(x + 47.5));

        if (k == 0)
        {
          rects.at(k).toElement().setAttribute("width", svgWidth);
          rects.at(k).toElement().setAttribute("height", svgHeight);
        }
        else
        {
          rects.at(k).toElement().setAttribute("height", 30.0);
        }
      }

      // number seller
      QDomElement textSeller = domDocument.createElement("text");
      QDomText textSellerContent = domDocument.createTextNode(QString("%1").arg(ConversionHelper::numberToString(i)));
      textSeller.setAttribute("x", 76);
      textSeller.setAttribute("y", 18);
      textSeller.setAttribute("text-anchor", "middle");
      textSeller.setAttribute("font-family", fontFamily);
      textSeller.setAttribute("font-size", fontSize);
      textSeller.setAttribute("font-weight", "bold");
      textSeller.setAttribute("fill", "#000000");
      textSeller.appendChild(textSellerContent);
      addHere.appendChild(textSeller);

      // number product
      QDomElement textProduct = domDocument.createElement("text");
      QDomText textProductContent = domDocument.createTextNode(QString("%1").arg(ConversionHelper::numberToString(j)));
      textProduct.setAttribute("x", 132);
      textProduct.setAttribute("y", 18);
      textProduct.setAttribute("text-anchor", "middle");
      textProduct.setAttribute("font-family", fontFamily);
      textProduct.setAttribute("font-size", fontSize);
      textProduct.setAttribute("font-weight", "bold");
      textProduct.setAttribute("fill", "#000000");
      textProduct.appendChild(textProductContent);
      addHere.appendChild(textProduct);

      /*// seperator between number and price
      QDomElement seperator = domDocument.createElement("rect");
      seperator.setAttribute("height", 1);
      seperator.setAttribute("width", svgWidth);
      seperator.setAttribute("x", 0);
      seperator.setAttribute("y", 60);
      addHere.appendChild(seperator);

      // price
      QDomElement textPreis = domDocument.createElement("text");
      QDomText textContentPreis = domDocument.createTextNode("Preis: ");
      textPreis.setAttribute("x", 30);
      textPreis.setAttribute("y", 84);
      textPreis.setAttribute("text-anchor", "middle");
      textPreis.setAttribute("font-family", fontFamily);
      textPreis.setAttribute("font-size", fontSize);
      textPreis.setAttribute("fill", "#000000");
      textPreis.appendChild(textContentPreis);
      addHere.appendChild(textPreis);*/

      // seperator between article and size
      int topPrize = 21;
      QDomElement seperator2 = domDocument.createElement("rect");
      seperator2.setAttribute("height", 1);
      seperator2.setAttribute("width", svgWidth);
      seperator2.setAttribute("x", 0);
      seperator2.setAttribute("y", topPrize);
      svg.appendChild(seperator2);

      // size
      QDomElement textSize = domDocument.createElement("text");
      QDomText textContentSize = domDocument.createTextNode("Größe:");
      textSize.setAttribute("x", 33);
      textSize.setAttribute("y", topPrize + 18);
      textSize.setAttribute("text-anchor", "left");
      textSize.setAttribute("font-family", fontFamily);
      textSize.setAttribute("font-size", fontSize);
      textSize.setAttribute("fill", "#000000");
      textSize.appendChild(textContentSize);
      svg.appendChild(textSize);

      // seperator between size and prize
      QDomElement seperator3 = domDocument.createElement("rect");
      seperator3.setAttribute("height", 1);
      seperator3.setAttribute("width", svgWidth);
      seperator3.setAttribute("x", 0);
      seperator3.setAttribute("y", topPrize + 21);
      svg.appendChild(seperator3);

      // price
      QDomElement textPreis = domDocument.createElement("text");
      QDomText textContentPreis = domDocument.createTextNode("Preis:");
      textPreis.setAttribute("x", 44);
      textPreis.setAttribute("y", topPrize + 38);
      textPreis.setAttribute("text-anchor", "left");
      textPreis.setAttribute("font-family", fontFamily);
      textPreis.setAttribute("font-size", fontSize);
      textPreis.setAttribute("fill", "#000000");
      textPreis.appendChild(textContentPreis);
      svg.appendChild(textPreis);

      // seperator between prize and code
      QDomElement seperator4 = domDocument.createElement("rect");
      seperator4.setAttribute("height", 1);
      seperator4.setAttribute("width", svgWidth);
      seperator4.setAttribute("x", 0);
      seperator4.setAttribute("y", topPrize + 41);
      svg.appendChild(seperator4);

      // vertical seperator
      QDomElement seperator5 = domDocument.createElement("rect");
      seperator5.setAttribute("height", svgHeight - 37);
      seperator5.setAttribute("width", 1);
      seperator5.setAttribute("x", svgWidth / 2);
      seperator5.setAttribute("y", 0);
      svg.appendChild(seperator5);

      QStringList style;
      style.append(QString("position: absolute"));
      style.append(QString("top:      %1px").arg(ConversionHelper::doubleToInt(globalMarginTop + (2 * globalMarginTop * (index / (lines * columns))) + marginTop + (index / columns) * height)));
      style.append(QString("left:     %1px").arg(ConversionHelper::doubleToInt(globalMarginLeft + marginLeft + (index % columns) * width)));
      //style.append(QString("border:   1px solid black")); //TODO remove after testing
      style.append(QString("height:   %1px").arg(height - marginTop  - marginBottom));
      style.append(QString("width:    %1px").arg(width  - marginLeft - marginRight));

      html.append(QString("<div style = \"%1\">").arg(style.join("; ")));

      QString svgString;
      QTextStream svgStream (&svgString);
      svg.save(svgStream, 2);
      html.append(svgString);

      html.append("</div>");
      ui->progressBar->setValue(progress * 100.0 / numberOfCodes);

      QApplication::processEvents();
      progress++;
      index++;
    }

    if (ui->checkBoxFillPages->isChecked())
    {
      int total = lines * columns;
      int offcut = numProducts % total;
      int fill = total - offcut;

      if (offcut != 0)
      {
        index += fill;

        /*for (int j = 0; j < fill; j++)
        {
          QDomDocument domDocument;
          QDomElement svg = domDocument.createElement("svg");
          svg.setAttribute("width", svgWidth);
          svg.setAttribute("height", svgHeight);
          svg.setAttribute("version", "1.1");

          domDocument.documentElement().appendChild(svg);

          // seller
          QDomElement textSeller = domDocument.createElement("text");
          QDomText textContentSeller = domDocument.createTextNode(QString("Verkäufer %1").arg(ConversionHelper::numberToString(i)));
          textSeller.setAttribute("x", 10);
          textSeller.setAttribute("y", 23);
          textSeller.setAttribute("text-anchor", "left");
          textSeller.setAttribute("font-family", fontFamily);
          textSeller.setAttribute("font-size", fontSize);
          textSeller.setAttribute("fill", "#000000");
          textSeller.appendChild(textContentSeller);
          svg.appendChild(textSeller);

          // seperator between seller and article
          QDomElement seperator = domDocument.createElement("rect");
          seperator.setAttribute("height", 1);
          seperator.setAttribute("width", svgWidth);
          seperator.setAttribute("x", 0);
          seperator.setAttribute("y", 27);
          svg.appendChild(seperator);

          // article
          QDomElement textArticle = domDocument.createElement("text");
          QDomText textContentArticle = domDocument.createTextNode("Artikel ");
          textArticle.setAttribute("x", 10);
          textArticle.setAttribute("y", 48);
          textArticle.setAttribute("text-anchor", "left");
          textArticle.setAttribute("font-family", fontFamily);
          textArticle.setAttribute("font-size", fontSize);
          textArticle.setAttribute("fill", "#000000");
          textArticle.appendChild(textContentArticle);
          svg.appendChild(textArticle);

          // seperator between article and size
          QDomElement seperator2 = domDocument.createElement("rect");
          seperator2.setAttribute("height", 1);
          seperator2.setAttribute("width", svgWidth);
          seperator2.setAttribute("x", 0);
          seperator2.setAttribute("y", 52);
          svg.appendChild(seperator2);

          // size
          QDomElement textSize = domDocument.createElement("text");
          QDomText textContentSize = domDocument.createTextNode("Größe ");
          textSize.setAttribute("x", 10);
          textSize.setAttribute("y", 73);
          textSize.setAttribute("text-anchor", "left");
          textSize.setAttribute("font-family", fontFamily);
          textSize.setAttribute("font-size", fontSize);
          textSize.setAttribute("fill", "#000000");
          textSize.appendChild(textContentSize);
          svg.appendChild(textSize);

          // seperator between size and prize
          QDomElement seperator3 = domDocument.createElement("rect");
          seperator3.setAttribute("height", 1);
          seperator3.setAttribute("width", svgWidth);
          seperator3.setAttribute("x", 0);
          seperator3.setAttribute("y", 77);
          svg.appendChild(seperator3);

          // price
          QDomElement textPreis = domDocument.createElement("text");
          QDomText textContentPreis = domDocument.createTextNode("Preis ");
          textPreis.setAttribute("x", 10);
          textPreis.setAttribute("y", 98);
          textPreis.setAttribute("text-anchor", "left");
          textPreis.setAttribute("font-family", fontFamily);
          textPreis.setAttribute("font-size", fontSize);
          textPreis.setAttribute("fill", "#000000");
          textPreis.appendChild(textContentPreis);
          svg.appendChild(textPreis);

          // vertical seperator
          QDomElement seperator4 = domDocument.createElement("rect");
          seperator4.setAttribute("height", svgHeight);
          seperator4.setAttribute("width", 1);
          seperator4.setAttribute("x", 128);
          seperator4.setAttribute("y", 0);
          svg.appendChild(seperator4);

          QStringList style;
          style.append(QString("position: absolute"));
          style.append(QString("top:      %1px").arg(ConversionHelper::doubleToInt(globalMarginTop + (2 * globalMarginTop * (index / (lines * columns))) + marginTop + (index / columns) * height)));
          style.append(QString("left:     %1px").arg(ConversionHelper::doubleToInt(globalMarginLeft + marginLeft + (index % columns) * width)));
          //style.append(QString("border:   1px solid black")); //TODO remove after testing
          style.append(QString("height:   %1px").arg(height - marginTop  - marginBottom));
          style.append(QString("width:    %1px").arg(width  - marginLeft - marginRight));

          html.append(QString("<div style = \"%1\">").arg(style.join("; ")));

          QString svgString;
          QTextStream svgStream (&svgString);
          svg.save(svgStream, 2);
          html.append(svgString);

          html.append("</div>");

          index++;
        }*/
      }
    }
  }
  ui->progressBar->setValue(100);

  html.append("</body></html>");

  ui->webView->setContent(html);
  ui->pushButtonStart->setEnabled(true);
}

bool MainWindow::toXml()
{
  QFile file(m_fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      return false;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(2);
    xml.writeStartDocument();

    xml.writeStartElement("Settings");

    xml.writeTextElement("SellerMin", QString("%1").arg(ui->spinBoxSellerMin->value()));
    xml.writeTextElement("SellerMax", QString("%1").arg(ui->spinBoxSellerMax->value()));
    xml.writeTextElement("ProductMin", QString("%1").arg(ui->spinBoxProductMin->value()));
    xml.writeTextElement("ProductMax", QString("%1").arg(ui->spinBoxProductMax->value()));
    xml.writeTextElement("FillPages", QString("%1").arg(ui->checkBoxFillPages->isChecked() ? "true" : "false"));

    xml.writeEndElement(); // Settings
    xml.writeEndDocument();

    file.close();

    return true;
}

bool MainWindow::fromXml()
{
  QFile file(m_fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      return false;
    }

    QXmlStreamReader xml(&file);

    if (!xml.readNextStartElement())
    {
      return false;
    }

    if (xml.name() != "Settings")
    {
      return false;
    }

    while (xml.readNextStartElement())
    {
      if (xml.name() == "SellerMin")
      {
        ui->spinBoxSellerMin->setValue(xml.readElementText().toInt());
      }
      else if (xml.name() == "SellerMax")
      {
        ui->spinBoxSellerMax->setValue(xml.readElementText().toInt());
      }
      else if (xml.name() == "ProductMin")
      {
        ui->spinBoxProductMin->setValue(xml.readElementText().toInt());
      }
      else if (xml.name() == "ProductMax")
      {
        ui->spinBoxProductMax->setValue(xml.readElementText().toInt());
      }
      else if (xml.name() == "FillPages")
      {
        ui->checkBoxFillPages->setChecked(xml.readElementText() == "true");
      }
      else
      {
        xml.skipCurrentElement();
      }
    }

    return true;
}
