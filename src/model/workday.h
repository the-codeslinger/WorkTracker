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

#ifndef WORKDAY_H
#define WORKDAY_H

#include "xmldata.h"
#include "datasource.h"
#include "worktask.h"

#include <QList>
#include <QMetaType>

#include <functional>

class QDateTime;
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
     * Creates a new `WorkDay` with a data source and a DOM node.
     */
    WorkDay(DataSource dataSource, const QDomElement& element);

    /*!
     * Creates a new instance with a data source, a start and a stop timestamp.
     */
    WorkDay(DataSource dataSource, const QDateTime& start, const QDateTime& stop);

    /*!
     * \return
     * Returns the start timestamp.
     */
    QDateTime start() const;

    /*!
     * Set a new `start` timestamp.
     */
    void setStart(const QDateTime& start);

    /*!
     * \return
     * Returns the stop timestamp.
     */
    QDateTime stop() const;

    /*!
     * Set a new `stop` timestamp.
     */
    void setStop(const QDateTime& stop);

    /*!
     * Add a completed `task` to the list of already completed tasks. If the task already
     * exists its values are overridden. If the work task has moved to a different task-id
     * then it will also be moved in the DOM document.
     */
    void addWorkTask(const WorkTask& task);
    
    /*!
     * Get a specific work-task.
     * 
     * \param task
     * Specifies the work-task's referencing task.
     * 
     * \return 
     * If found the work-task is returned or a null-instance otherwise.
     */
    WorkTask findWorkTask(const Task& task) const;
    
    /*!
     * \return
     * Generates a list of `WorkTask` instances attached to this work-day.
     */ 
    QList<WorkTask> workTasks() const;

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
    WorkTask activeWorkTask() const;

    /*!
     * \return
     * Returns a list of distinct tasks this workday is comprised of.
     */
    QList<Task> distinctTasks() const;

    /*!
     * \return
     * Returns the total amount of seconds that has been spent on all finished tasks
     * combined.
     */
    int totalTime() const;

    /*!
     * Fetches the work-task at the specified index position.
     * 
     * \return
     * If the index is out of bounds a null-work-task is returned. Otherwise the 
     * work-task at the specified index position is returned.
     */
    WorkTask at(int index) const;

    /*!
     * \return
     * Returns the number of work-task elements in this work-day.
     */
    int countWorkTasks() const;

protected:
    QString elementName() const;

private:
    /*!
     * Iterate the existing work-tasks on the DOM level and call a function for every
     * individual work-task. This shall reduce iterating several lists, first the DOM to
     * create a list of work-tasks and then that list to accomplish a specific task.
     */
    void iterateWorkTasks(std::function<void(const WorkTask&)> predicate) const;
    
    /*!
     * Iterate the exisiting work-tasks on the DOM level and return the one for which
     * `predicate` returns true.
     */
    WorkTask findWorkTask(std::function<bool(const WorkTask&)> predicate) const;
};

Q_DECLARE_METATYPE(WorkDay)

#endif // WORKDAY_H
