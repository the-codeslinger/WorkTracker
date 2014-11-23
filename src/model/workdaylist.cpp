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

#include "workdaylist.h"
#include "workday.h"

#include <QDateTime>

static const QString g_elementName = "workdays";

WorkDayList::WorkDayList()
    : XmlData()
{ }

WorkDayList::WorkDayList(const QDomDocument& dataSource)
    : XmlData(dataSource, QDomElement())
{
    findOrCreateElement();
}

void 
WorkDayList::setDataSource(const QDomDocument& dataSource)
{
    XmlData::setDataSource(dataSource);
    findOrCreateElement();
}

int 
WorkDayList::size() const
{
    return m_element.childNodes().size();
}

WorkDay 
WorkDayList::findLastOpen() const
{
    auto children = m_element.childNodes();

    // Fetch the last item of the list which is the only one that can still be unfinished
    int count = children.count();
    if (count == 0) {
        return WorkDay();
    }

    QDomNode day = children.item(count - 1);
    if (day.isNull() || !day.isElement()) {
        return WorkDay();
    }

    auto element = day.toElement();
    auto stop = attributeDateTime("stop", element);
    if (!stop.isNull()) {
        return WorkDay();
    }
    
    // No "stop" attribute means that this day is still ongoing.
    return WorkDay(m_dataSource, element);
}

WorkDay
WorkDayList::at(int index) const
{
    auto nodes = m_element.childNodes();
    QDomNode workDay = nodes.at(index);
    if (workDay.isElement()) {
        return WorkDay(m_dataSource, workDay.toElement());
    }
    return WorkDay();
}

void 
WorkDayList::findOrCreateElement()
{
    QDomElement root = m_dataSource.documentElement();
    if (!root.isNull()) {
        m_element = root.firstChildElement(g_elementName);
        if (m_element.isNull()) {
            m_element = m_dataSource.createElement(g_elementName);
            root.appendChild(m_element);
        }
    }
}