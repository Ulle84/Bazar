#include <QDate>
#include <QDateTime>

#include "DateTimeHelper.h"

QString DateTimeHelper::getCurrentDate()
{
    return QDate::currentDate().toString("yyyy-MM-dd");
}

QString DateTimeHelper::getCurrentDateAndTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

QString DateTimeHelper::getCurrentDateGerman()
{
    return QDateTime::currentDateTime().toString("dd.MM.yyyy");
}

QString DateTimeHelper::getCurrentTimeGerman()
{
    return QDateTime::currentDateTime().toString("hh:mm");
}
