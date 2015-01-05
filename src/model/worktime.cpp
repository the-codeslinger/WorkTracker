/*
 * Copyright 2014 Robert Lohr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
