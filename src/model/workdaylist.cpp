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

#include "workdaylist.h"
#include "workday.h"

#include <QDateTime>

#include <algorithm>

static const QString g_elementName = "workdays";

WorkDayList::WorkDayList()
    : XmlData{}
{ }

WorkDayList::WorkDayList(DataSource dataSource)
    : XmlData{std::move(dataSource), QDomElement{}}
{
    findOrCreateElement();
}

void 
WorkDayList::setDataSource(DataSource dataSource)
{
    XmlData::setDataSource(std::move(dataSource));
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
    auto count = children.count();
    if (count == 0) {
        return WorkDay();
    }

    auto day = children.item(count - 1);
    if (day.isNull() || !day.isElement()) {
        return WorkDay{};
    }

    auto element = day.toElement();
    auto stop = attributeDateTime("stop", element);
    if (!stop.isNull()) {
        return WorkDay{};
    }
    
    // No "stop" attribute means that this day is still ongoing.
    return WorkDay{m_dataSource, element};
}

WorkDay
WorkDayList::at(int index) const
{
    auto nodes   = m_element.childNodes();
    auto workDay = nodes.at(index);
    if (workDay.isElement()) {
        return WorkDay{m_dataSource, workDay.toElement()};
    }
    return WorkDay{};
}

void 
WorkDayList::findOrCreateElement()
{
    auto root = m_dataSource.document().documentElement();
    if (!root.isNull()) {
        m_element = root.firstChildElement(g_elementName);
        if (m_element.isNull()) {
            m_element = m_dataSource.document().createElement(g_elementName);
            root.appendChild(m_element);
        }
    }
}
