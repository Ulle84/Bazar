#ifndef CONVERSIONHELPER_H
#define CONVERSIONHELPER_H

#include <QString>

class ConversionHelper
{
public:
    static int doubleToInt(double value);
    static QString numbersToString(int i1, int i2);
    static QString numberToString(int i1);
};

#endif // CONVERSIONHELPER_H
