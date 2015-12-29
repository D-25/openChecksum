#include "datacalc.h"
#include <iostream>
#include <QObject>

// To recall timeCalc. We could use it to calculate the data-size, in the future.

QString timeCalc(qint64 elapsed)
{
    int timeSecond = elapsed / 1000;
    int timeMinute = (timeSecond / 60) % 60;
    int timeHour = timeSecond / 3600;
    timeSecond = timeSecond % 60;

    QString timeString = (QString("%1:%2:%3")
                        .arg(timeHour, 2, 10, QLatin1Char('0'))
                        .arg(timeMinute, 2, 10, QLatin1Char('0'))
                        .arg(timeSecond, 2, 10, QLatin1Char('0')));

    if (timeString == "00:00:00")
    {
        timeString = QObject::tr("meno di un secondo");
    }

    if (elapsed == -1)
    {
        timeString = QObject::tr("<i>nessun dato disponibile.</i>");
    }

    return timeString;
}
