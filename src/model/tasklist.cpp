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

#include "tasklist.h"
#include "task.h"
#include "../helper.h"

#include <algorithm>

static const QString g_elementName = "tasks";

TaskList::TaskList(const QDomDocument& dataSource)
    : XmlData(dataSource, QDomElement())
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

void 
TaskList::setDataSource(const QDomDocument& dataSource)
{
    XmlData::setDataSource(dataSource);
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
        QString value = element.attribute("id");
        if (!value.isNull()) {
            bool ok = false;
            int foundId = value.toInt(&ok);
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
        return Task();
    }

    auto found = findFirstNode(m_element.childNodes(), predicate);
    if (!found.isNull()) {
        return Task(m_dataSource, found);
    }
    return Task();
}

void 
TaskList::findOrCreateElement()
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