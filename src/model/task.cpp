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

#include "task.h"

#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>

int Task::invalidId = -1;

Task::Task()
    : XmlData()
{
}

Task::Task(const QDomDocument& p_dataSource)
    : XmlData(p_dataSource)
{
    createNode(Task::invalidId, "", QDate());
}

Task::Task(const QDomDocument& p_dataSource, const QString& p_name, 
           const QDate& p_lastUsed)
    : XmlData(p_dataSource)
{
    createNode(Task::invalidId, p_name, p_lastUsed);
}

Task::Task(const QDomDocument& p_dataSource, int p_id, const QString& p_name, 
           const QDate& p_lastUsed)
    : XmlData(p_dataSource)
{
    createNode(p_id, p_name, p_lastUsed);
}

Task::Task(const QDomDocument& p_dataSource, const QDomElement& p_node)
    : XmlData(p_dataSource, p_node)
{
}

Task::Task(const Task& p_other)
    : XmlData(p_other)
{
}

int
Task::id() const
{
    return attributeInt("id");
}

void
Task::setId(int p_id)
{
    setAttribute("id", p_id);
}

QString
Task::name() const
{
    return attributeString("name");
}

void
Task::setName(const QString& p_name)
{
    setAttribute("name", p_name);
    if (!p_name.isEmpty()) {
        addToList();
    }
}

QDate
Task::lastUsed() const
{
    return attributeDate("last_used");
}

void
Task::setLastUsed(const QDate& p_lastUsed)
{
    setAttribute("last_used", p_lastUsed);
}

void
Task::createNode(int p_id, const QString& p_name, const QDate& p_lastUsed)
{
    // Check if a node already exists. If so, re-use it instead of creating a new one with
    // the same name/id. If name and id don't match: programmer error.
    if (0 <= p_id) {
        *this = Task::get(p_id, m_dataSource);
    }
    else if (!p_name.isEmpty()) {
        *this = Task::findByName(p_name, m_dataSource);
    }
    
    // Only create a node if we didn't find (search) any before
    if (m_node.isNull()) {
        m_node = m_dataSource.createElement("item");
        
        // No need to set any values if there is no name. Also no need to attach it to the
        // list of tasks.
        if (!p_name.isEmpty()) {
            if (0 >= p_id) {
                // Generate an id if there is none
                p_id = Task::count(m_dataSource);
            }
            setAttribute("id",        p_id);
            setAttribute("name",      p_name);
            setAttribute("last_used", p_lastUsed);
            
            addToList();
        }
    }
}

void 
Task::addToList()
{
    // The parent in this case is the <tasks> DOM element that contains all the task
    // items. This is not a parent as we define it for our relations, so it is not 
    // explicitly saved in m_parent.
    if (m_node.parentNode().isNull()) {
        QDomElement root = m_dataSource.documentElement();
        QDomNode tasks = root.namedItem("tasks");
        if (tasks.isNull()) {
            tasks = m_dataSource.createElement("tasks");
            root.appendChild(tasks);
        }
        
        tasks.appendChild(m_node);
    }
}

/* * * * * * * * * * * * static methods * * * * * * * * * * * */

Task
Task::get(int id, const QDomDocument& dataSource)
{
    return findTask(dataSource, [id](Task task) {
        return task.id() == id;
    });
}


Task
Task::findByName(const QString& p_name, const QDomDocument& p_dataSource)
{
    return findTask(p_dataSource, [p_name](Task task) {
        return task.name() == p_name;
    });
}

QList<Task>
Task::list(const QDomDocument& p_dataSource)
{
    QList<Task> tasks;

    QDomNodeList children = getTaskNodes(p_dataSource);
    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode node = children.item(c);
        if (node.isNull() || !node.isElement()) {
            continue;
        }

        tasks.append(Task(p_dataSource, node.toElement()));
    }

    return tasks;
}

int
Task::count(const QDomDocument& p_dataSource)
{
    QDomNodeList children = getTaskNodes(p_dataSource);
    return children.count();
}

Task
Task::findTask(const QDomDocument& p_dataSource, std::function<bool(Task)> p_predicate)
{
    QDomNodeList children = getTaskNodes(p_dataSource);
    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode node = children.item(c);
        if (node.isNull() || !node.isElement()) {
            continue;
        }

        Task item = Task(p_dataSource, node.toElement());
        if (p_predicate(item)) {
            return item;
        }
    }

    return Task();
}

QDomNodeList
Task::getTaskNodes(const QDomDocument& p_dataSource)
{
    QDomElement root  = p_dataSource.documentElement();
    if (root.isNull()) {
        return QDomNodeList();
    }

    QDomNode tasks = root.firstChildElement("tasks");
    if (tasks.isNull()) {
        return QDomNodeList();
    }

    return tasks.childNodes();
}
