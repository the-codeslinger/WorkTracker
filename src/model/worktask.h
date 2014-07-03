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

#include <QList>
#include <QDateTime>
#include <QPair>

class QDomNode;
class QDomDocument;

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
     * Creates a new `Task` with a data source.
     */
    WorkTask(QDomDocument* dataSource);

    /*!
     * Creates a new `Task` with a data source, the DOM node, a task and its start and
     * stop timestamp.
     */
    WorkTask(QDomDocument* dataSource, QDomElement node, Task task, QDateTime start,
             QDateTime stop);

    /*!
     * Calls the base class' implementation and immediately adds start and stop as well.
     */
    void setNode(QDomNode node);

    /*!
     * \return
     * Returns the task that is assigned.
     */
    Task task() const;

    /*!
     * Set a new `task`.
     */
    void setTask(Task task);

    /*!
     * \return
     * Returns the start timestamp.
     */
    QDateTime start() const;

    /*!
     * Set a new `start` timestamp.
     */
    void setStart(QDateTime start);

    /*!
     * \return
     * Returns the start timestamp.
     */
    QDateTime stop() const;

    /*!
     * Set a new `stop` timestamp.
     */
    void setStop(QDateTime stop);

    /*!
     * \return
     * Return the total amount of seconds from start to stop.
     */
    int totalTime() const;

    /*!
     * \return
     * Returns `true` if `WorkTask::m_task` returns `true` or `false` otherwise.
     *
     * \see
     * Task::isNull()
     */
    bool isNull() const;

    /*!
     * Resets the complete worktask by invalidating the timestamps task.
     */
    void clear();

    /**
     * Two tasks are considered equal if they refer to the same DOM node.
     *
     * @param other
     * The other task to compare to.
     *
     * @return
     * `true` if both refer to the same DOM node or `false` if not.
     */
    bool operator==(const WorkTask& other) const;

    /*!
     * Reads all the individual timestamps from the <task> XML node inside a <day> node.
     *
     * \param node
     * The <task> XML node that contains all the <time> nodes.
     *
     * \param dataSource
     * The database to read from.
     *
     * \return
     * `WorkTask` instances are created for every <time> XML node and returned in a list.
     * If no <time> nodes are present then the list returned is empty.
     */
    static QList<WorkTask> fromDomNode(QDomNode* node, QDomDocument* dataSource);

private:
    /*!
     * The task value of the worktask.
     */
    Task m_task;
    /*!
     * The start timestamp of the worktask.
     */
    QDateTime m_start;
    /*!
     * The stop timestamp of the worktask.
     */
    QDateTime m_stop;

    /*!
     * Helper method that extracts the value of the id attribute of the task from the
     * node.
     */
    static int idFromAttr(QDomNode* attr);

    /*!
     * Helper method that extracts the timestamp value from the attribute from the node.
     */
    static QDateTime timestampFromAttr(QDomNode* attr);
};

#endif // WORKTASK_H
