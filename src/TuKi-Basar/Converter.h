#ifndef CONVERTER_H
#define CONVERTER_H

#include <QString>

class Converter
{
public:
    static QString intToQString(int number);
    static QString prizeToString(double prize, bool leadingSpace = true);
};

#endif // CONVERTER_H
