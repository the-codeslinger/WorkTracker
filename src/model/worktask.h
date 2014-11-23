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

#ifndef WORKTASK_H
#define WORKTASK_H

#include "task.h"
#include "xmldata.h"
#include "worktime.h"

#include <QList>

/*!
 * \brief Combines a `Task` with a start and a stop time, making it one unit of work.
 *
 * A `WorkDay` consists of multiple `WorkTask` items, whereas more than just one
 * `WorkTask` can reference the same `Task`. The sum of all worktasks combined make up
 * the total time of work spent on a single task.
 *
 * This class is a hybrid between an direct link to aa DOM node via `XmlData` as well as
 * a plain old C++ class in that is has member variables. This class is created by
 * entities that do not interact with the DOM and thus need a temporary storage for
 * information, which is in member variables. As soon as a DOM node is associated the
 * data is written to that node as well.
 */
class WorkTask : public XmlData
{
public:
    /*!
     * Create a new `WorkTask` instance without any values.
     */
    WorkTask();
    
    /*!
     * Creates a new instance with based on the DOM element.
     */
    WorkTask(const QDomDocument& dataSource, const QDomElement& element);
    
    /*!
     * Creates a new instance with a task and the parent to assign the node to.
     */
    WorkTask(const QDomDocument& dataSource, const Task& task);
    
    /*!
     * Copy constructor.
     */
    WorkTask(const WorkTask& other);

    /*!
     * \return
     * Returns the task that is assigned.
     */
    Task task() const;

    /*!
     * Set a new `task`.
     */
    void setTask(const Task& task);
    
    /*!
     * Add a time element to the work-task's list of times. This is just a more aptly 
     * named wrapper around `XmlData::appendChild(const XmlData&)`.
     * 
     * \param time
     * The time element to add. This sets the parent of `time` to be the work-task.
     */
    void addTime(const WorkTime& time);
    
    /*!
     * \return 
     * Returns the list of work-times that are part of this work-task.
     */
    QList<WorkTime> times() const;

    /*!
     * \return
     * Return the total amount of seconds from start to stop of all work-times combined.
     */
    qint64 timeInSeconds() const;
    
    /*!
     * \return 
     * Returns `true` if this work-task contains an work-time without a stop timestamp or
     * `false` if all work-times have a stop timestamp.
     */
    bool isActiveTask() const;
    
    /*!
     * \return 
     * Returns the currently active work-time or a null-work-time if there is none without
     * a stop timestamp.
     */
    WorkTime activeTime() const;

protected:
    QString elementName() const;
};

#endif // WORKTASK_H
