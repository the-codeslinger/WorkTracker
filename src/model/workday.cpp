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

WorkDay::WorkDay()
    : XmlData(nullptr)
{
}

WorkDay::WorkDay(QDomDocument* dataSource)
    : XmlData(dataSource)
{
    createNode(QDateTime(), QDateTime());
}

WorkDay::WorkDay(QDomDocument* dataSource, QDomElement node)
    : XmlData(dataSource, node)
{
}

WorkDay::WorkDay(QDomDocument* dataSource, QDateTime start)
    : XmlData(dataSource)
{
    createNode(start, QDateTime());
}

WorkDay::WorkDay(QDomDocument* dataSource, QDateTime start, QDateTime stop)
    : XmlData(dataSource)
{
    createNode(start, stop);
}

QDateTime
WorkDay::start() const
{
    return attributeValue("start").toDateTime();
}

void
WorkDay::setStart(QDateTime start)
{
    addAttribute("start", start.toString(Qt::ISODate));
}

QDateTime
WorkDay::stop() const
{
    return attributeValue("stop").toDateTime();
}

void
WorkDay::setStop(QDateTime stop)
{
    addAttribute("stop", stop.toString(Qt::ISODate));
}

WorkTask
WorkDay::addTask(WorkTask task)
{
    // Find the original parent DOM node (= the real task) based on the currently known
    // task's node
    QDomElement origWorkTask;
    QDomNode workTaskNode = task.node();
    if (!workTaskNode.isNull()) {
        // Parent cannot be null if the node is valid (ignoring tampering of the database
        // by users)
        origWorkTask = workTaskNode.parentNode().toElement();
    }

    bool appendTask = workTaskNode.isNull();

    // Find or create the current parent DOM node based on the associated task-id
    QDomElement workTask = findTask(task.task().id());
    if (workTask.isNull()) {
        // Create a new work task
        workTask = m_dataSource->createElement("task");
        workTask.setAttribute("id", task.task().id());

        m_node.appendChild(workTask);
    }

    // If the task has no node assigned yet, then it can't have any parent yet. This means
    // this particular work task is not yet known.
    QDomElement workTaskElem;
    if (workTaskNode.isNull()) {
        workTaskElem = m_dataSource->createElement("time");
        workTask.appendChild(workTaskElem);
    }
    else {
        // If the original and current task are not equal then we need to remove the
        // task's node from the original one and move it to the new node. If they are the
        // same then we don't need to do anything.

        // The original work task cannot be null if we get to this point, but better be
        // safe than sorry.
        if (!origWorkTask.isNull() && workTask != origWorkTask) {
            // This automatically reparents the node from its old parent to the new
            workTaskNode = workTask.appendChild(workTaskNode);

            if (workTaskNode.isNull()) {
                // Reparenting didn't work
                return WorkTask();
            }

            // As is the case with the parent node, we have to replace the task first
            // pushed into m_tasks with the updated one.
            auto pos = std::find_if(std::begin(m_tasks), std::end(m_tasks),
                         [task](const WorkTask& item){
                return task.node() == item.node();
            });

            if (pos != std::end(m_tasks)) {
                m_tasks.erase(pos);
            }

            appendTask = true;
        }

        workTaskElem = workTaskNode.toElement();
    }

    // Now we can simply set the values and be sure it's on the right node
    workTaskElem.setAttribute("start", task.start().toString(Qt::ISODate));
    workTaskElem.setAttribute("stop",  task.stop().toString(Qt::ISODate));

    task.setNode(workTaskElem);

    // Only append if this is a new task (=start a new task) or an existing one was
    // replaced by a different task (=stop task)
    if (appendTask) {
        m_tasks.append(task);
    }

    return task;
}

void
WorkDay::clear()
{
    XmlData::clear();
    m_tasks.clear();
}

void
WorkDay::createNode(QDateTime start, QDateTime stop)
{
    m_node = m_dataSource->createElement("workday");

    addAttribute("start", start.toString(Qt::ISODate));
    addAttribute("stop",  stop.toString(Qt::ISODate));
}

QDomElement
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
            QVariant idValue = attributeValueFromNode(node, "id");

            bool ok = false;
            int foundId = idValue.toInt(&ok);
            if (ok && foundId == id) {
                return node.toElement();
            }
        }
    }

    return QDomElement();
}

QString
WorkDay::generateSummary() const
{
    QString value;
    QTextStream html(&value);
    html << "<html>"
         << "<head><title>WorkTracker Summary</title></head>"
         << "<body><ul>";

    QMap<QString, int> durations;
    for (WorkTask task : m_tasks) {
        QMap<QString, int>::iterator found = durations.find(task.task().name());
        if (found == durations.end()) {
            durations.insert(task.task().name(), task.totalTime());
        }
        else {
            int& totalTime = found.value();
            totalTime += task.totalTime();
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
WorkDay::findLastOpen(QDomDocument* dataSource)
{
    QDomElement root = dataSource->documentElement();
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
        return fromDomNode(day.toElement(), dataSource);
    }

    return WorkDay();
}

WorkDay
WorkDay::fromDomNode(QDomElement node, QDomDocument* dataSource)
{
    QDomNamedNodeMap attributes = node.attributes();
    QDomAttr dateAttr = attributes.namedItem("start").toAttr();
    if (dateAttr.isNull()) {
        return WorkDay();
    }

    // There's at least a start date so let's treat it as a valid (although maybe
    // incomplete) day object
    QDateTime start = QDateTime::fromString(dateAttr.value(), Qt::ISODate);
    QDateTime stop;

    // If stop is also present then we'll take that as well. If it's missing this
    // probably means the application was shut down before finishing a day.
    dateAttr = attributes.namedItem("stop").toAttr();
    if (!dateAttr.isNull()) {
        stop = QDateTime::fromString(dateAttr.value(), Qt::ISODate);
    }

    WorkDay day(dataSource, node);

    QDomNodeList children = node.childNodes();
    int count = children.count();

    for (int c = 0; c < count; c++) {
        QDomNode taskNode = children.item(c);
        if (!taskNode.isNull()) {
            QList<WorkTask> tasks = WorkTask::fromDomNode(&taskNode, dataSource);
            day.m_tasks.append(tasks);
        }
    }

    return day;
}

WorkTask
WorkDay::runningWorkTask() const
{
    for (WorkTask task : m_tasks) {
        if (!task.stop().isValid()) {
            return task;
        }
    }

    return WorkTask();
}

int
WorkDay::totalTime() const
{
    int duration = 0;
    for (WorkTask task : m_tasks) {
        duration += task.totalTime();
    }
    return duration;
}
