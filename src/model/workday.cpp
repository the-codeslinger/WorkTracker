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

#include "workday.h"
#include "task.h"
#include "../helper.h"

#include <QTextStream>
#include <QMapIterator>
#include <QDateTime>

#include <algorithm>

static const QString g_elementName = "workday";

WorkDay::WorkDay()
    : XmlData{}
{ }


WorkDay::WorkDay(DataSource dataSource, const QDomElement& node)
    : XmlData{std::move(dataSource), node}
{
}

WorkDay::WorkDay(DataSource dataSource, const QDateTime& start, const QDateTime& stop)
    : XmlData{std::move(dataSource), g_elementName}
{
    setAttribute("start", start);
    setAttribute("stop",  stop);
}

QDateTime
WorkDay::start() const
{
    return attributeDateTime("start");
}

void
WorkDay::setStart(const QDateTime& start)
{
    setAttribute("start", start);
}

QDateTime
WorkDay::stop() const
{
    return attributeDateTime("stop");
}

void
WorkDay::setStop(const QDateTime& stop)
{
    setAttribute("stop", stop);
}

void
WorkDay::addWorkTask(const WorkTask& task)
{
    appendChild(task);
}

WorkTask 
WorkDay::findWorkTask(const Task& task) const
{
    return findWorkTask([task](const WorkTask& workTask) {
        return workTask.task().id() == task.id();
    });
}

QString
WorkDay::generateSummary() const
{
    auto value = QString{};
    QTextStream html{&value};
    html << "<html>"
         << "<head><title>WorkTracker Summary</title></head>"
         << "<body><ul>";

    auto durations = QMap<QString, int>{};
    iterateWorkTasks([&durations](const WorkTask& workTask) {
        auto task = workTask.task();
        
        auto found = durations.find(task.name());
        if (found == durations.end()) {
            durations.insert(task.name(), workTask.timeInSeconds());
        }
        else {
            auto& totalTime = found.value();
            totalTime += workTask.timeInSeconds();
        }
    });

    auto iter = QMapIterator<QString, int>{durations};
    while (iter.hasNext()) {
        auto item = iter.next();

        auto minutes = item.value() / 60.0f;
        auto hours   = minutes      / 60.0f;

        html << "<li><i>" << item.key() << "</i>: ";
        html << roundTwoDecimals(hours) << " h (" << qRound(minutes) << " min)";
        html << "</li>";
    }

    html << "</ul></body></html>";

    return value;
}

WorkTask
WorkDay::activeWorkTask() const
{
    return findWorkTask([](const WorkTask& workTask) {
        return workTask.isActiveTask();
    });
}

int
WorkDay::totalTime() const
{
    auto duration = 0;
    iterateWorkTasks([&duration](const WorkTask& workTask) {
        duration += workTask.timeInSeconds();
    });
    return duration;
}

QList<Task>
WorkDay::distinctTasks() const
{
    auto tasks = QList<Task>{};
    iterateWorkTasks([&tasks](const WorkTask& workTask) {
        auto task  = workTask.task();
        auto found = std::find_if(std::begin(tasks), std::end(tasks),
                                  [task](const Task& knownTask) {
            return task.id() == knownTask.id();
        });

        if (found == std::end(tasks)) {
            tasks << task;
        }
    });
    return tasks;
}

QList<WorkTask> 
WorkDay::workTasks() const
{
    auto tasks = QList<WorkTask>{};
    
    forEachNode(m_element.childNodes(), [&tasks, this](const QDomElement& element) {
        tasks << WorkTask(m_dataSource, element);
    });
    
    return tasks;
}

void 
WorkDay::iterateWorkTasks(std::function<void(const WorkTask&)> predicate) const
{
    forEachNode(m_element.childNodes(), [this, predicate](const QDomElement& element) {
        if ("task" == element.nodeName()) {
            predicate(WorkTask(m_dataSource, element));
        }
    });
}

WorkTask 
WorkDay::findWorkTask(std::function<bool(const WorkTask&)> predicate) const
{
    auto element = findFirstNode(m_element.childNodes(), 
                                 [this, predicate](const QDomElement& element) {
        if ("task" == element.nodeName()) {
            WorkTask workTask(m_dataSource, element);
            return predicate(workTask);
        }
        return false;
    });
    if (!element.isNull()) {
        return WorkTask(m_dataSource, element);
    }
    
    return WorkTask{};
}

WorkTask 
WorkDay::at(int index) const
{
    auto nodes = m_element.childNodes();
    if (index < 0 || index >= nodes.count()) {
        return WorkTask{};
    }

    auto node = nodes.at(index);
    if (node.isElement()) {
        return WorkTask{m_dataSource, node.toElement()};
    }

    return WorkTask{};
}

int 
WorkDay::countWorkTasks() const
{
    auto count = 0;
    forEachNode(m_element.childNodes(), [&count](const QDomElement&) {
        count++;
    });
    return count;
}
