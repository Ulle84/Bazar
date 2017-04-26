#ifndef BARCODEGENERATOR_H
#define BARCODEGENERATOR_H

#include <QString>
#include <QByteArray>

class BarCodeGenerator
{
public:
  BarCodeGenerator();

  bool generateFile(const QString& input, QString outputFile);
  bool generateSvg(const QString& input, QByteArray& output);

private:
  const int m_codeNumber;
};

#endif // BARCODEGENERATOR_H
