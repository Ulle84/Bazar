#include "Settings.h"
#include "ui_Settings.h"

#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
//#include <QVariant>

Settings::Settings(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Settings)
{
  m_fileName = "Settings.xml";
  ui->setupUi(this);
  fromXml();
}

Settings::~Settings()
{
  delete ui;
}

int Settings::getPc()
{
  return ui->spinBoxPc->value();
}

int Settings::getArticleMin()
{
  return ui->spinBoxArticleMin->value();
}

int Settings::getArticleMax()
{
  return ui->spinBoxArticleMax->value();
}

int Settings::getSellerMin()
{
  return ui->spinBoxSellerMin->value();
}

int Settings::getSellerMax()
{
  return ui->spinBoxSellerMax->value();
}

double Settings::getDeductionPercentage()
{
  return ui->doubleSpinBoxDeductionPercentage->value();
}

void Settings::setPc(int value)
{
  ui->spinBoxPc->setValue(value);
}

void Settings::setArticleMin(int value)
{
  ui->spinBoxArticleMin->setValue(value);
}

void Settings::setArticleMax(int value)
{
  ui->spinBoxArticleMax->setValue(value);
}

void Settings::setSellerMin(int value)
{
  ui->spinBoxSellerMin->setValue(value);
}

void Settings::setSellerMax(int value)
{
  ui->spinBoxSellerMax->setValue(value);
}

void Settings::setDeductionPercentage(double value)
{
  ui->doubleSpinBoxDeductionPercentage->setValue(value);
}

bool Settings::fromXml()
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

  if (xml.name() != "TuKiBazarSettings")
  {
    return false;
  }

  while (xml.readNextStartElement())
  {
    if (xml.name() == "Pc")
    {
      setPc(xml.readElementText().toInt());
    }
    else if (xml.name() == "SellerMin")
    {
      setSellerMin(xml.readElementText().toInt());
    }
    else if (xml.name() == "SellerMax")
    {
      setSellerMax(xml.readElementText().toInt());
    }
    else if (xml.name() == "ProductMin")
    {
      setArticleMin(xml.readElementText().toInt());
    }
    else if (xml.name() == "ProductMax")
    {
      setArticleMax(xml.readElementText().toInt());
    }
    else if (xml.name() == "DeductionPercentage")
    {
      setDeductionPercentage(xml.readElementText().toDouble());
    }
    else
    {
      xml.skipCurrentElement();
    }
  }

  return true;
}

bool Settings::toXml()
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

  xml.writeStartElement("TuKiBazarSettings");

  xml.writeTextElement("Pc", QString("%1").arg(getPc()));
  xml.writeTextElement("SellerMin", QString("%1").arg(getSellerMin()));
  xml.writeTextElement("SellerMax", QString("%1").arg(getSellerMax()));
  xml.writeTextElement("ProductMin", QString("%1").arg(getArticleMin()));
  xml.writeTextElement("ProductMax", QString("%1").arg(getArticleMax()));
  xml.writeTextElement("DeductionPercentage", QString("%1").arg(getDeductionPercentage()));

  xml.writeEndElement(); // TuKiBazarSettings
  xml.writeEndDocument();

  file.close();

  return true;
}

void Settings::on_pushButtonOk_clicked()
{
  if (ui->spinBoxSellerMax->value() < ui->spinBoxSellerMin->value())
  {
    QMessageBox mb;
    mb.setText(tr("Die maximale Verkäufernummer darf nicht kleiner als die minimale Verkäufernummer sein!"));
    mb.exec();
    return;
  }

  if (ui->spinBoxArticleMax->value() < ui->spinBoxArticleMin->value())
  {
    QMessageBox mb;
    mb.setText(tr("Die maximale Produktnummer darf nicht kleiner als die minimale Produktnummer sein!"));
    mb.exec();
    return;
  }

  toXml();

  this->accept();
}
