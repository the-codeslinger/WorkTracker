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

#include <QTextStream>
#include <QMapIterator>
#include <QDateTime>

#include <algorithm>

static const QString g_elementName = "workday";

WorkDay::WorkDay()
    : XmlData()
{ }


WorkDay::WorkDay(DataSource dataSource, const QDomElement& node)
    : XmlData(std::move(dataSource), node)
{
}

WorkDay::WorkDay(DataSource dataSource, const QDateTime& start, const QDateTime& stop)
    : XmlData(std::move(dataSource), g_elementName)
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
    QString value;
    QTextStream html(&value);
    html << "<html>"
         << "<head><title>WorkTracker Summary</title></head>"
         << "<body><ul>";

    QMap<QString, int> durations;
    iterateWorkTasks([&durations](const WorkTask& workTask) {
        Task task = workTask.task();
        
        QMap<QString, int>::iterator found = durations.find(task.name());
        if (found == durations.end()) {
            durations.insert(task.name(), workTask.timeInSeconds());
        }
        else {
            int& totalTime = found.value();
            totalTime += workTask.timeInSeconds();
        }
    });

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
    int duration = 0;
    iterateWorkTasks([&duration](const WorkTask& workTask) {
        duration += workTask.timeInSeconds();
    });
    return duration;
}

QList<Task>
WorkDay::distinctTasks() const
{
    QList<Task> tasks;
    iterateWorkTasks([&tasks](const WorkTask& workTask) {
        Task task = workTask.task();
        
        auto found = std::find_if(std::begin(tasks), std::end(tasks),
                                  [task](const Task& task) {
            return task.id() == task.id();
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
    QList<WorkTask> tasks;
    
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
    
    return WorkTask();
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