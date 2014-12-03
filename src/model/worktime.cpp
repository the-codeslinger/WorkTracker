#include "worktime.h"

#include <QDateTime>

#include <algorithm>

static const QString g_elementName = "time";

WorkTime::WorkTime()
    : XmlData()
{ }

WorkTime::WorkTime(DataSource dataSource, const QDomElement& element)
    : XmlData(std::move(dataSource), element)
{ }

WorkTime::WorkTime(DataSource dataSource, const QDateTime& start, const QDateTime& stop)
    : XmlData(std::move(dataSource), g_elementName)
{
    setAttribute("start", start);
    setAttribute("stop",  stop);
}

WorkTime::WorkTime(const WorkTime& other)
    : XmlData(other)
{ }

QDateTime 
WorkTime::start() const
{
    return attributeDateTime("start");
}

void 
WorkTime::setStart(const QDateTime& start)
{
    setAttribute("start", start);
}

QDateTime 
WorkTime::stop() const
{
    return attributeDateTime("stop");
}

void 
WorkTime::setStop(const QDateTime& stop)
{
    setAttribute("stop", stop);
}

qint64
WorkTime::timeInSeconds() const
{
    QDateTime startTime = start();
    QDateTime stopTime  = stop();
    
    qint64 totalTime = 0;
    if (startTime.isValid()) {
        if (stopTime.isNull()) {
            stopTime = QDateTime::currentDateTimeUtc();
        }
        
        totalTime = startTime.secsTo(stopTime);
    }
    
    return totalTime;
}

QString 
WorkTime::elementName() const
{
    return g_elementName;
}
