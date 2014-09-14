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

#include "workday.h"
#include "task.h"
#include "../helper.h"

#include <QDomDocument>
#include <QTextStream>
#include <QMapIterator>
#include <QDebug>

#include <algorithm>

WorkDay::WorkDay()
    : XmlData()
{
}

WorkDay::WorkDay(const QDomDocument& p_dataSource)
    : XmlData(p_dataSource)
{
    createNode(QDateTime(), QDateTime());
}

WorkDay::WorkDay(const QDomDocument& p_dataSource, const QDomElement& p_node)
    : XmlData(p_dataSource, p_node)
{
}

WorkDay::WorkDay(const QDomDocument& p_dataSource, const QDateTime& p_start)
    : XmlData(p_dataSource)
{
    createNode(p_start, QDateTime());
}

WorkDay::WorkDay(const QDomDocument& p_dataSource, const QDateTime& p_start, 
                 const QDateTime& p_stop)
    : XmlData(p_dataSource)
{
    createNode(p_start, p_stop);
}

QDateTime
WorkDay::start() const
{
    return attributeDateTime("start");
}

void
WorkDay::setStart(const QDateTime& p_start)
{
    setAttribute("start", p_start);
}

QDateTime
WorkDay::stop() const
{
    return attributeDateTime("stop");
}

void
WorkDay::setStop(const QDateTime& p_stop)
{
    setAttribute("stop", p_stop);
}

void
WorkDay::addWorkTask(const WorkTask& p_task)
{
    QDomNode workTask = p_task.node();
    if (!workTask.isNull() && !m_node.isNull()) {
        m_node.appendChild(workTask);
    }
}

WorkTask 
WorkDay::workTask(const Task& p_task) const
{
    QList<WorkTask> tasks = workTasks();
    for (const WorkTask& workTask : tasks) {
        if (workTask.task().id() == p_task.id()) {
            return workTask;
        }
    }
    return WorkTask();
}

void
WorkDay::createNode(const QDateTime& p_start, const QDateTime& p_stop)
{
    m_node = m_dataSource.createElement("workday");

    setAttribute("start", p_start);
    setAttribute("stop",  p_stop);
}

QString
WorkDay::generateSummary() const
{
    QString value;
    QTextStream html(&value);
    html << "<html>"
         << "<head><title>WorkTracker Summary</title></head>"
         << "<body><ul>";

    QList<WorkTask> tasks = workTasks();
    
    QMap<QString, int> durations;
    for (WorkTask task : tasks) {
        QMap<QString, int>::iterator found = durations.find(task.task().name());
        if (found == durations.end()) {
            durations.insert(task.task().name(), task.timeInSeconds());
        }
        else {
            int& totalTime = found.value();
            totalTime += task.timeInSeconds();
        }
    }

    QMapIterator<QString, int> iter(durations);
    while (iter.hasNext()) {
        auto item = iter.next();

        float minutes = item.value() / 60.0f;
        float hours   = minutes      / 60.0f;

        html << "<li><i>" << item.key() << "</i>: ";
        html << roundTwoDecimals(hours) << " h (" << qRound(minutes) << " min)";
        html << "</li>";
    }

    html << "</ul></body></html>";

    return value;
}

WorkDay
WorkDay::findLastOpen(const QDomDocument& p_dataSource)
{
    QDomElement root = p_dataSource.documentElement();
    QDomElement days = root.firstChildElement("workdays");

    QDomNodeList children = days.childNodes();

    // Fetch the last item of the list which is the only one that can still be unfinished
    int count = children.count();
    if (count == 0) {
        return WorkDay();
    }

    QDomNode day = children.item(count - 1);
    if (day.isNull() || !day.isElement()) {
        return WorkDay();
    }

    QDomNamedNodeMap attributes = day.attributes();
    QDomAttr stopAttr = attributes.namedItem("stop").toAttr();
    if (stopAttr.isNull()) {
        // No "stop" attribute means that this day is still ongoing
        return fromDomNode(day.toElement(), p_dataSource);
    }

    return WorkDay();
}

WorkDay
WorkDay::fromDomNode(const QDomElement& p_node, const QDomDocument& p_dataSource)
{
    QDomNamedNodeMap attributes = p_node.attributes();
    QDomAttr dateAttr = attributes.namedItem("start").toAttr();
    if (dateAttr.isNull()) {
        return WorkDay();
    }
    
    return WorkDay(p_dataSource, p_node);
}

WorkTask
WorkDay::runningWorkTask() const
{
    QList<WorkTask> tasks = workTasks();
    
    for (WorkTask task : tasks) {
        if (task.isActiveTask()) {
            return task;
        }
    }

    return WorkTask();
}

int
WorkDay::totalTime() const
{
    QList<WorkTask> tasks = workTasks();
    
    int duration = 0;
    for (WorkTask task : tasks) {
        duration += task.timeInSeconds();
    }
    return duration;
}

int
WorkDay::count(const QDomDocument& p_dataSource)
{
    QDomNodeList nodes = getWorkDayNodes(p_dataSource);
    return nodes.size();
}

WorkDay
WorkDay::at(int p_index, const QDomDocument& p_dataSource)
{
    QDomNodeList nodes = getWorkDayNodes(p_dataSource);
    QDomNode workday = nodes.at(p_index);
    return fromDomNode(workday.toElement(), p_dataSource);
}

QDomNodeList
WorkDay::getWorkDayNodes(const QDomDocument& p_dataSource)
{
    QDomElement root  = p_dataSource.documentElement();
    if (root.isNull()) {
        return QDomNodeList();
    }

    QDomNode workdays = root.firstChildElement("workdays");
    if (workdays.isNull()) {
        return QDomNodeList();
    }

    return workdays.childNodes();
}

QList<Task>
WorkDay::distinctTasks() const
{
    QList<WorkTask> tasks = workTasks();
    
    QList<Task> workTasks;
    for (const WorkTask& worktask : tasks) {
        auto found = std::find_if(std::begin(workTasks), std::end(workTasks),
                                  [worktask](const Task& p_task) {
            return worktask.task().id() == p_task.id();
        });

        if (found == std::end(workTasks)) {
            workTasks << worktask.task();
        }
    }
    return workTasks;
}

QList<WorkTask> 
WorkDay::workTasks() const
{
    QList<WorkTask> tasks;
            
    QDomNodeList children = m_node.childNodes();
    int count = children.size();
    for (int c = 0; c < count; c++) {
        QDomNode node = children.at(c);
        if ("task" == node.nodeName() && node.isElement()) {
            tasks << WorkTask(m_dataSource, node.toElement(), m_node);
        }
    }
    
    return tasks;
}
