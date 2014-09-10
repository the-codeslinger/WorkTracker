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
     * Creates a new `WorkTask` with a data source. An associated DOM node is also created
     * but cannot be added to the DOM until a parent, the `WorkDay`, is set.
     * 
     * \param p_dataSource
     * The XML DOM.
     */
    WorkTask(QDomDocument* p_dataSource);
    
    /*!
     * Creates a new `WorkTask` with a data source and the associated DOM node from which
     * to read the data and write it to.
     * 
     * \param p_dataSource
     * The XML DOM.
     * 
     * \param p_node
     * The work-task node inside the work-day.
     */
    WorkTask(QDomDocument* p_dataSource, QDomElement p_node);
    
    /*!
     * Creates a new `WorkTask` with the associated DOM node and the values of a task and
     * start plus stop times.
     * 
     * \param p_dataSource
     * The XML DOM.
     * 
     * \param p_parent
     * The node of the work-day to which to attach this work-task.
     * 
     * \param p_task
     * The actual `Task`.
     * 
     * \param p_start
     * The starting timestamp of the work-task.
     * 
     * \param p_stop
     * The stopped timestamp of the work-task.
     */
    WorkTask(QDomDocument* p_dataSource, QDomNode p_parent, Task p_task, 
             QDateTime p_start, QDateTime p_stop);
    
    /*!
     * Sets a new parent for the work-task. This either sets an initial parent and 
     * attaches the work-task to the DOM or reparents to the new node.
     * 
     * \param p_parent
     * The new work-day to attach the work-task to. If this is not a work-day then no
     * action will be performed.
     */
    void setParent(QDomNode p_parent);

    /*!
     * \return
     * Returns the task that is assigned.
     */
    Task task() const;

    /*!
     * Set a new `task`.
     */
    void setTask(Task p_task);

    /*!
     * \return
     * Returns the start timestamp.
     */
    QDateTime start() const;

    /*!
     * Set a new `start` timestamp.
     */
    void setStart(QDateTime p_start);

    /*!
     * \return
     * Returns the start timestamp.
     */
    QDateTime stop() const;

    /*!
     * Set a new `stop` timestamp.
     */
    void setStop(QDateTime p_stop);

    /*!
     * \return
     * Return the total amount of seconds from start to stop.
     */
    int totalTime() const;

    /*!
     * \return
     * Returns `true` if `Task::isNull()` returns `true` or `false` otherwise.
     *
     * \see
     * Task::isNull()
     */
    bool isNull() const;

    /*!
     * Resets the complete worktask by invalidating the timestamps and task. 
     */
    void clear();

    /*!
     * Reads all the individual timestamps from the <task> XML node inside a <day> node.
     *
     * \param p_node
     * The <task> XML node that contains all the <time> nodes.
     *
     * \param p_dataSource
     * The database to read from.
     *
     * \return
     * `WorkTask` instances are created for every <time> XML node and returned in a list.
     * If no <time> nodes are present then the list returned is empty.
     */
    static QList<WorkTask> fromDomNode(QDomNode* p_node, QDomDocument* p_dataSource);

private:
    /*!
     * Creates the work-task DOM node on the parent and sets the values.
     * 
     * \param p_parent
     * The node of the work-day to which to attach this work-task.
     * 
     * \param p_task
     * The actual `Task`.
     * 
     * \param p_start
     * The starting timestamp of the work-task.
     * 
     * \param p_stop
     * The stopped timestamp of the work-task.
     */
    void createNode(QDomNode p_parent, Task p_task, QDateTime p_start, QDateTime p_stop);

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
