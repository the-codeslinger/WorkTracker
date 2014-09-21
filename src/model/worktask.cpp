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

#include <QDomNodeList>

WorkTask::WorkTask()
    : XmlData()
{
}

WorkTask::WorkTask(const QDomDocument& p_dataSource)
    : XmlData(p_dataSource)
{
    createNode(QDomNode(), Task());
}

WorkTask::WorkTask(const QDomDocument& p_dataSource, const QDomElement& p_node, 
                   const QDomNode& p_parent)
    : XmlData(p_dataSource, p_node, p_parent)
{
}

WorkTask::WorkTask(const QDomDocument& p_dataSource, const QDomNode& p_parent, 
                   const Task& p_task)
    : XmlData(p_dataSource)
{
    createNode(p_parent, p_task);
}

WorkTask::WorkTask(const WorkTask& p_other)
    : XmlData(p_other)
{
}

void 
WorkTask::setDataSource(const QDomDocument &p_dataSource)
{
    XmlData::setDataSource(p_dataSource);
    createNode(QDomNode(), Task());
}

Task
WorkTask::task() const
{
    int id = attributeInt("id");
    Task task;
    if (-1 != id) {
        task = Task::get(id, m_dataSource);
    }
    return task;
}

void
WorkTask::setTask(const Task& p_task)
{
    if (!p_task.isNull()) {
        setAttribute("id", p_task.id());
    }
}

void 
WorkTask::addTime(const WorkTime& p_time)
{
    if (!p_time.isNull() && !m_node.isNull()) {
        m_node.appendChild(p_time.node());
    }
}

QList<WorkTime> 
WorkTask::workTimes() const
{
    QList<WorkTime> goodTimes;
    
    QDomNodeList children = m_node.childNodes();
    int count = children.size();
    for (int c = 0; c < count; c++) {
        QDomNode child = children.at(c);
        if (child.isElement()) {
            goodTimes << WorkTime(m_dataSource, child.toElement(), m_node);
        }
    }
    
    return goodTimes;
}

qint64
WorkTask::timeInSeconds() const
{
    qint64 totalTime = 0;
    
    QList<WorkTime> times = workTimes();
    for (const WorkTime& time : times) {
        totalTime += time.timeInSeconds();
    }
            
    return totalTime;
}

void 
WorkTask::createNode(const QDomNode& p_parent, const Task& p_task)
{
    if (m_dataSource.isNull() || !m_node.isNull()) {
        return;
    }
    
    m_parent = p_parent;
    m_node   = m_dataSource.createElement("task");
    
    if (!m_parent.isNull()) {
        m_parent.appendChild(m_node);
    }
    
    if (!p_task.name().isEmpty()) {
        if (0 < p_task.id()) {
            
        }
        setAttribute("id", p_task.id());
    }
}

bool
WorkTask::isActiveTask() const
{
    QList<WorkTime> times = workTimes();
    for (const WorkTime& time : times) {
        if (time.stop().isNull()) {
            return true;
        }
    }
    return false;
}

WorkTime 
WorkTask::activeWorkTime() const
{
    QList<WorkTime> times = workTimes();
    for (const WorkTime& time : times) {
        if (time.stop().isNull()) {
            return time;
        }
    }
    return WorkTime();
}
