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

#include "tasklist.h"
#include "task.h"
#include "../helper.h"

#include <algorithm>
#include <QDate>

static const QString g_elementName = "tasks";

TaskList::TaskList()
    : XmlData{}
{ }

TaskList::TaskList(DataSource dataSource)
    : XmlData{std::move(dataSource), QDomElement{}}
{
    findOrCreateElement();
}

void 
TaskList::setDataSource(DataSource dataSource)
{
    XmlData::setDataSource(std::move(dataSource));
    findOrCreateElement();
}

int 
TaskList::size() const
{
    return m_element.childNodes().size();
}

void 
TaskList::add(Task task)
{
    if (XmlData::invalidId == task.id()) {
        // Counting starts at zero, thus the current size is the currently highest id 
        // plus one.
        task.setId(size());
        appendChild(task);
    }
}

Task 
TaskList::find(int id) const
{
    return find([id](const QDomElement& element) {
        auto value = element.attribute("id");
        if (!value.isNull()) {
            auto ok = false;
            auto foundId = value.toInt(&ok);
            if (ok) {
                return id == foundId;
            }
        }
        return false;
    });
}

Task 
TaskList::find(const QString& name) const
{
    return find([name](const QDomElement& element) {
        return name.toLower() == element.attribute("name").toLower();
    });
}

Task 
TaskList::find(std::function<bool(QDomElement)> predicate) const
{
    if (m_element.isNull()) {
        return Task{};
    }

    auto found = findFirstNode(m_element.childNodes(), predicate);
    if (!found.isNull()) {
        return Task{m_dataSource, found};
    }
    return Task{};
}

void 
TaskList::findOrCreateElement()
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

QList<Task>
TaskList::tasksSortedByLastUsed() const
{
    auto list = QList<Task>{};

    auto children = m_element.childNodes();
    
    int size = children.size();
    for (int i = 0; i < size; i++) {
        auto taskNode = children.at(i);
        if (taskNode.isElement()) {
            list << Task{ m_dataSource, taskNode.toElement() };
        }
    }

    std::sort(std::begin(list), std::end(list), [](const Task& left, const Task& right) {
        return left.lastUsed() > right.lastUsed();
    });

    return list;
}
