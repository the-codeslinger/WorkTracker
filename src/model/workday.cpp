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
    /*// Find or create the current parent DOM node based on the associated task-id
    QDomElement workTask = findTask(p_task.task().id());
    if (workTask.isNull()) {
        // Create a new work task
        workTask = m_dataSource.createElement("task");
        workTask.setAttribute("id", p_task.task().id());

        m_node.appendChild(workTask);
    }

    // Find the original parent DOM node (= the real task) based on the currently known
    // task's node
    int index = m_tasks.indexOf(p_task);
    if (-1 == index) {
        // This work task has just been started. Create a new DOM node
        QDomElement workTaskElem = m_dataSource.createElement("time");
        workTask.appendChild(workTaskElem);

        //p_task.setNode(workTaskElem);
        m_tasks.append(p_task);
    }
    else {
        // Here we can assume that the task found in the list has a DOM element associated
        // with it (created when added to the list)
        WorkTask& wota = m_tasks[index];

        QDomNode    workTaskNode = wota.node();
        QDomElement origWorkTask = workTaskNode.parentNode().toElement();

        if (workTask != origWorkTask) {
            // This automatically reparents the node from its old parent to the new
            workTaskNode = workTask.appendChild(workTaskNode);
            if (workTaskNode.isNull()) {
                // Reparenting didn't work
            }

            // Now set the node on the reference object retrieved from the list. Fetching
            // the task as reference from the list avoids removing it and then append it
            // again with the new values. This way we modify it in place.
            //wota.setNode(workTaskNode);
            wota.setTask(p_task.task());
        }

        //wota.setStop(task.stop());
    }*/
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

/*QDomElement
WorkDay::findTask(int id) const
{
    QDomNodeList tasks = m_node.childNodes();
    if (tasks.isEmpty()) {
        return QDomElement();
    }

    int count = tasks.count();
    for (int c = 0; c < count; c++) {
        QDomNode node = tasks.item(c);
        if (!node.isNull() && node.isElement()) {
            QVariant idValue;// = attributeValueFromNode(node, "id");

            bool ok = false;
            int foundId = idValue.toInt(&ok);
            if (ok && foundId == id) {
                return node.toElement();
            }
        }
    }

    return QDomElement();
}*/

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

/*QList<WorkTask>
WorkDay::workTasks(const Task& p_task) const
{
    QList<WorkTask> tasks = workTasks();
            
    QList<WorkTask> worktasks;
    for (const WorkTask& worktask : tasks) {
        if (worktask.task().id() == p_task.id()) {
            worktasks << worktask;
        }
    }
    return worktasks;
}*/

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
