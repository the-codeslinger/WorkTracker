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

#include "worktask.h"
#include "task.h"
#include "tasklist.h"
#include "../helper.h"

#include <QDateTime>

#include <algorithm>

static const QString g_elementName = "task";

WorkTask::WorkTask()
    : XmlData{}
{ }

WorkTask::WorkTask(DataSource dataSource, const QDomElement& element)
    : XmlData{std::move(dataSource), element}
{ }

WorkTask::WorkTask(DataSource dataSource, const Task& task)
    : XmlData{std::move(dataSource), g_elementName}
{
    setTask(task);
}

WorkTask::WorkTask(const WorkTask& other)
    : XmlData{other}
{ }

Task
WorkTask::task() const
{
    auto id = attributeInt("id");
    if (XmlData::invalidId != id) {
        auto tl = TaskList{m_dataSource};
        return tl.find(id);
    }
    return Task{};
}

void
WorkTask::setTask(const Task& task)
{
    if (!task.isNull()) {
        setAttribute("id", task.id());
    }
}

void 
WorkTask::addTime(const WorkTime& time)
{
    appendChild(time);
}

QList<WorkTime> 
WorkTask::times() const
{
    auto goodTimes = QList<WorkTime>{};
    
    auto children = m_element.childNodes();
    forEachNode(children, [&goodTimes, this](const QDomElement& element) {
        goodTimes << WorkTime{m_dataSource, element};
    });
    
    return goodTimes;
}

qint64
WorkTask::timeInSeconds() const
{
    auto totalTime = qint64{0};

    forEach(times(), [&totalTime](const WorkTime& time) {
        totalTime += time.timeInSeconds();
    });
            
    return totalTime;
}

bool
WorkTask::isActiveTask() const
{
    auto list = times();
    return std::end(list) != findIf(list, [](const WorkTime& time) {
        return time.stop().isNull();
    });
}

WorkTime 
WorkTask::activeTime() const
{
    // Looks like GCC 4.8.2 isn't able to correctly deduce the type like MSVC 2013 (or
    // the latter just behaves incorrectly).
    return firstOrDefault<QList<WorkTime>>(times(), [](const WorkTime& time) {
        return time.stop().isNull();
    });
}

QString 
WorkTask::elementName() const
{
    return g_elementName;
}
