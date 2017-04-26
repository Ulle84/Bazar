#include "PercentageCalculator.h"

QString PercentageCalculator::getPercentage(int complete, int part)
{
  double percentage = part * 100.0 / complete;
  return QString::number(percentage, 'f', 1).replace('.', ',').append(" %");
}
