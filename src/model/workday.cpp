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
    // Find or create the current parent DOM node based on the associated task-id
    QDomElement workTask = findTask(task.task().id());
    if (workTask.isNull()) {
        // Create a new work task
        workTask = m_dataSource->createElement("task");
        workTask.setAttribute("id", task.task().id());

        m_node.appendChild(workTask);
    }

    // Find the original parent DOM node (= the real task) based on the currently known
    // task's node
    int index = m_tasks.indexOf(task);
    if (-1 == index) {
        // This work task has just been started. Create a new DOM node
        QDomElement workTaskElem = m_dataSource->createElement("time");
        workTask.appendChild(workTaskElem);

        task.setNode(workTaskElem);
        m_tasks.append(task);

        return task;
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
                return WorkTask();
            }

            // Now set the node on the reference object retrieved from the list. Fetching
            // the task as reference from the list avoids removing it and then append it
            // again with the new values. This way we modify it in place.
            wota.setNode(workTaskNode);
            wota.setTask(task.task());
        }

        wota.setStop(task.stop());

        return wota;
    }
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

int
WorkDay::count(QDomDocument* p_dataSource)
{
    QDomNodeList nodes = getWorkDayNodes(p_dataSource);
    return nodes.size();
}

WorkDay
WorkDay::at(int p_index, QDomDocument* p_dataSource)
{
    QDomNodeList nodes = getWorkDayNodes(p_dataSource);
    QDomNode workday = nodes.at(p_index);
    return fromDomNode(workday.toElement(), p_dataSource);
}

QDomNodeList
WorkDay::getWorkDayNodes(QDomDocument* p_dataSource)
{
    QDomElement root  = p_dataSource->documentElement();
    if (root.isNull()) {
        return QDomNodeList();
    }

    QDomNode workdays = root.firstChildElement("workdays");
    if (workdays.isNull()) {
        return QDomNodeList();
    }

    return workdays.childNodes();
}
