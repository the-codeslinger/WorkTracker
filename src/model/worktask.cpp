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

#include "worktask.h"
#include "task.h"

#include <QDomNode>
#include <QDomNodeList>

WorkTask::WorkTask()
    : XmlData(nullptr)
{
}

WorkTask::WorkTask(QDomDocument* p_dataSource)
    : XmlData(p_dataSource)
{
    // This results in only XmlData::m_node being created without being attached to the
    // DOM tree.
    createNode(QDomNode(), Task(), QDateTime(), QDateTime());
}

WorkTask::WorkTask(QDomDocument* p_dataSource, QDomElement p_node)
    : XmlData(p_dataSource, p_node)
{
}

WorkTask::WorkTask(QDomDocument* p_dataSource, QDomNode p_parent, Task p_task, 
                   QDateTime p_start, QDateTime p_stop)
    : XmlData(p_dataSource)
{
    createNode(p_parent, p_task, p_start, p_stop);
}

Task
WorkTask::task() const
{
    return Task();
}

void
WorkTask::setTask(Task p_task)
{
    
}

QDateTime
WorkTask::start() const
{
    if (m_node.isNull()) {
        //return m_start;
    }
    else {
        //return attributeValue("start").toDateTime();
    }
    return QDateTime();
}

void
WorkTask::setStart(QDateTime start)
{
    /*m_start = start;
    if (!m_node.isNull()) {
        addAttribute("start", start.toString(Qt::ISODate));
    }*/
}

QDateTime
WorkTask::stop() const
{
    if (m_node.isNull()) {
        //return m_stop;
    }
    else {
        //return attributeValue("stop").toDateTime();
    }
    return QDateTime();
}

void
WorkTask::setStop(QDateTime stop)
{
    /*m_stop = stop;
    if (!m_node.isNull()) {
        addAttribute("stop", stop.toString(Qt::ISODate));
    }*/
}

int
WorkTask::totalTime() const
{
    /*QDateTime stopped = stop();
    if (!stopped.isValid()) {
        stopped = QDateTime::currentDateTimeUtc();
    }
    return start().secsTo(stopped);*/
    return 0;
}

bool
WorkTask::isNull() const
{
    return true;
}

void
WorkTask::clear()
{
    /*m_task.clear();
    addAttribute("start", "");
    addAttribute("stop", "");*/
}

QList<WorkTask>
WorkTask::fromDomNode(QDomNode* node, QDomDocument* dataSource)
{
    QDomNamedNodeMap attributes = node->attributes();

    QDomNode attrNode = attributes.namedItem("id");
    int id = idFromAttr(&attrNode);
    if (Task::invalidId == id) {
        return QList<WorkTask>();
    }

    Task task = Task::get(id, dataSource);
    if (task.isNull()) {
        return QList<WorkTask>();
    }

    // Fetch and add the time objects
    QDomNodeList children = node->childNodes();

    QList<WorkTask> workTasks;

    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode timeNode = children.item(c);
        attributes = timeNode.attributes();

        attrNode = attributes.namedItem("start");
        QDateTime start = timestampFromAttr(&attrNode);

        attrNode = attributes.namedItem("stop");
        QDateTime stop  = timestampFromAttr(&attrNode);

        // Only start needs to be valid. Stop may be empty which makes this an ongoing
        // task (maybe a sudden shutdown of the application).
        if (start.isValid()) {
            workTasks.append(WorkTask(dataSource, timeNode.toElement(), task, start,
                                      stop));
        }
    }

    return workTasks;
}

QDateTime
WorkTask::timestampFromAttr(QDomNode* attr)
{
    QDomAttr a = attr->toAttr();
    if (a.isNull()) {
        return QDateTime();
    }

    return QDateTime::fromString(a.value(), Qt::ISODate);
}

int
WorkTask::idFromAttr(QDomNode* attr)
{
    QDomAttr a = attr->toAttr();
    if (a.isNull()) {
        return Task::invalidId;
    }

    bool ok = false;
    int id = a.value().toInt(&ok);
    if (!ok) {
        return Task::invalidId;
    }

    return id;
}

void 
WorkTask::createNode(QDomNode p_parent, Task p_task, QDateTime p_start, QDateTime p_stop)
{
    m_node = m_dataSource->createElement("time");
    
    if (!p_parent.isNull()) {
        p_parent.appendChild(m_node);
    }
    
    if (!p_task.isNull()) {
        
    }
}
