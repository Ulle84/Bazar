#include <QProcess>
#include <QDebug>

#include "BarCodeGenerator.h"

BarCodeGenerator::BarCodeGenerator() :
  m_codeNumber(7)
{
}

bool BarCodeGenerator::generateFile(const QString& input, QString outputFile)
{
  QString program = "zint.exe";
  QStringList arguments;
  arguments << "--scale=1.6"
            << "--barcode=20"
            << "--notext"
            << QString("-o %1").arg(outputFile)
            << QString("-d%1").arg(input);

  return QProcess::execute(program, arguments) == QProcess::NormalExit;
}

bool BarCodeGenerator::generateSvg(const QString& input, QByteArray& output)
{
  QString program = "zint.exe";
  QStringList arguments;
  arguments << "--directsvg"
            << "--scale=1.6"
            << "--barcode=20"
            << "--notext"
            << QString("-d%1").arg(input);

  QProcess zint;
  zint.start(program, arguments);

  if (!zint.waitForStarted())
  {
    return false;
  }

  zint.closeWriteChannel();

  if (!zint.waitForFinished())
  {
    return false;
  }

  output = zint.readAll();

  return true;
}
