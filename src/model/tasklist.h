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

#ifndef TASKLIST_H
#define TASKLIST_H

#include "xmldata.h"
#include "datasource.h"
#include <functional>

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

private:
    Task find(std::function<bool(QDomElement)> predicate) const;
    void findOrCreateElement();
};

#endif