/*
 * Copyright 2023 Robert Lohr
 *
 * This file is part of WorkTracker.
 *
 * WorkTracker is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * WorkTracker is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with
 * WorkTracker. If not, see <https://www.gnu.org/licenses/>.
 */

#include "worktime.h"

#include <QDateTime>

#include <algorithm>

static const QString g_elementName = "time";

WorkTime::WorkTime()
    : XmlData{}
{ }

WorkTime::WorkTime(DataSource dataSource, const QDomElement& element)
    : XmlData{std::move(dataSource), element}
{ }

WorkTime::WorkTime(DataSource dataSource, const QDateTime& start, const QDateTime& stop)
    : XmlData{std::move(dataSource), g_elementName}
{
    setAttribute("start", start);
    setAttribute("stop",  stop);
}

WorkTime::WorkTime(const WorkTime& other)
    : XmlData{other}
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
    auto startTime = start();
    auto stopTime  = stop();
    
    auto totalTime = qint64{0};
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
