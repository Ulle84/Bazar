#ifndef DATETIMEHELPER_H
#define DATETIMEHELPER_H

#include <QString>

class DateTimeHelper
{
public:
    static QString getCurrentDate();
    static QString getCurrentDateAndTime();

    static QString getCurrentDateGerman();
    static QString getCurrentTimeGerman();
};

#endif // DATETIMEHELPER_H
