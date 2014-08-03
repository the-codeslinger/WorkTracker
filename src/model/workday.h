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

#ifndef WORKDAY_H
#define WORKDAY_H

#include "worktask.h"
#include "xmldata.h"

#include <QDateTime>
#include <QList>
#include <QDomElement>
#include <QMetaType>

class QDomDocument;

/*!
 * \brief A `WorkDay` consists of one or more `WorkTask` items which in turn contain the
 *        `Task` items.
 *
 * A workday is a collection of all the tasks the user has performed throughout a day of
 * work, identified by a start-timestamp. Each task itself is represented by a `WorkTask`
 * item that collects the specific `Task` and the start and stop timestamps.
 */
class WorkDay : public XmlData
{
public:
    /*!
     * Creates a new `WorkDay` instance without any values.
     */
    WorkDay();

    /*!
     * Creates a new `WorkDay` with a data source
     */
    WorkDay(QDomDocument* dataSource);

    /*!
     * Creates a new `WorkDay` with a data source and a DOM node.
     */
    WorkDay(QDomDocument* dataSource, QDomElement node);

    /*!
     * Creates a new instance with a data source and a start timestamp.
     */
    WorkDay(QDomDocument* dataSource, QDateTime start);

    /*!
     * Creates a new instance with a data source, a start and a stop timestamp.
     */
    WorkDay(QDomDocument* dataSource, QDateTime start, QDateTime stop);

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
     * Returns the stop timestamp.
     */
    QDateTime stop() const;

    /*!
     * Set a new `stop` timestamp.
     */
    void setStop(QDateTime stop);

    /*!
     * Add a completed `task` to the list of already completed tasks. If the task already
     * exists its values are overridden. If the work task has moved to a different task-id
     * then it will also be moved in the DOM document.
     *
     * \return
     * Returns the task instance that was passed in as a parameter associated with an
     * XML node.
     */
    WorkTask addTask(WorkTask task);

    /*!
     * Resets the complete workday by invalidating the timestamps and removing all work
     * tasks.
     */
    void clear();

    /*!
     * Generates an HTML summary over all the added work task items.
     *
     * \return
     * The HTML string returned looks the following:
     *
     * \code
     * <html>
     *   <head><title>WorkTracker Summary</title></head>
     *   <body>
     *     <ul>
     *       <li><i>Task Name:</i> Duration hours (duration minutes)</li>
     *       ...
     *     </ul>
     *   </body>
     * </html>
     * \endcode
     */
    QString generateSummary() const;

    /*!
     * Searches the list of tasks to find one that is not yet finished.
     *
     * \return
     * If there's a running task (one with an invalid stop-date) then this task is
     * returned, otherwise a null-task.
     */
    WorkTask runningWorkTask() const;

    /*!
     * \return
     * Returns a list of distinct tasks this workday is comprised of.
     */
    QList<Task> distinctTasks() const;

    /*!
     * \return
     * Returns all `WorkTask` items that match the given `Task`.
     */
    QList<WorkTask> workTasks(Task p_task) const;

    /*!
     * \return
     * Returns the total amount of seconds that has been spent on all finished tasks
     * combined.
     */
    int totalTime() const;

    /*!
     * Checks the database if the last workday has not been finished and loads it.
     *
     * \param dataSource
     * The database from which to load the latest unfinished workday.
     *
     * \return
     * If the last workday is not unfinished then a null-workday is returned, otherwise
     * the workday and all its task items is loaded.
     */
    static WorkDay findLastOpen(QDomDocument* dataSource);

    /*!
     * Get the number of workdays in the database.
     *
     * \param p_dataSource
     * The database that contains the list of tasks.
     *
     * \return
     * Returns the number of workdays in the database.
     */
    static int count(QDomDocument* p_dataSource);

    /*!
     * Returns the workday that can be found at the specified position in the DOM tree.
     *
     * \param p_index
     * The position in the DOM tree.
     *
     * \param p_dataSource
     * The database that contains the list of tasks.
     *
     * \return
     * If the `p_index` is valid then a valid workday is returned. Otherwise the workday
     * instance returned is null.
     */
    static WorkDay at(int p_index, QDomDocument* p_dataSource);

private:
    /*!
     * The list of work task items.
     */
    QList<WorkTask> m_tasks;

    /*!
     * Create a new DOM node and store it in `XmlData::m_node`.
     */
    void createNode(QDateTime start, QDateTime stop);

    /*!
     * Searches the DOM tree for the specified worktask item. This is used by
     * `addTask(WorkTask)` to append a new set of timestamps to the correct part of the
     * DOM or determine if a new worktask item needs to be created.
     */
    QDomElement findTask(int id) const;

    /*!
     * Create a new `WorkDay` from the DOM node.
     */
    static WorkDay fromDomNode(QDomElement node, QDomDocument* dataSource);

    /*!
     * Convenience method that hides some of the ugly DOM API.
     */
    static QDomNodeList getWorkDayNodes(QDomDocument* p_dataSource);
};

Q_DECLARE_METATYPE(WorkDay)

#endif // WORKDAY_H
