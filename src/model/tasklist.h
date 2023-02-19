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

#ifndef TASKLIST_H
#define TASKLIST_H

#include "xmldata.h"
#include "datasource.h"

#include <functional>
#include <QList>

class Task;

/*!
 * Represents the list of tasks in the database. Methods to search for tasks and add new
 * tasks are provided.
 */
class TaskList : public XmlData
{
public:
    /*!
     * Creates a new task list that cannot be reasonably used until 
     * `TaskList::setDataSource(const QDomDocument&)` is called.
     */
    TaskList();

    /*!
     * Creates a new instance with a data source. If the <tasks> element doesn't exist it
     * is created.
     */
    TaskList(DataSource dataSource);
    
    /*!
     * Overrides the base class' version to call `TaskList::findOrCreateElement()` after
     * setting the new data source.
     */
    void setDataSource(DataSource dataSource);

    /*!
     * Returns the number of tasks in the task list.
     */
    int size() const;

    /*!
     * Append a task. If the tasks already has an id it is ignored. Otherwise a new id
     * will be generated.
     */
    void add(Task task);

    /*! 
     * Search for a task by its id.
     */
    Task find(int id) const;

    /*!
     * Search for a task by its name. Name's are compared case insensitive.
     */
    Task find(const QString& name) const;

    /*!
     * Returns a list of all the names of tasks sorted by their last used date.
     */
    QList<Task> tasksSortedByLastUsed() const;

private:
    Task find(std::function<bool(QDomElement)> predicate) const;
    void findOrCreateElement();
};

#endif
