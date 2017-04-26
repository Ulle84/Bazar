#include "ConversionHelper.h"



int ConversionHelper::doubleToInt(double value)
{
    value > 0 ? value += 0.5 : value -=  0.5;
    return value;
}

QString ConversionHelper::numbersToString(int i1, int i2)
{
  return numberToString(i1) + numberToString(i2);
}

QString ConversionHelper::numberToString(int i1)
{
  QString s1 = QString("%1").arg(i1);
  if (i1 < 10)
  {
    s1.prepend("0");
  }
  if (i1 < 100)
  {
    s1.prepend("0");
  }
  return s1;
}
