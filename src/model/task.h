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

#ifndef TASK_H
#define TASK_H

#include "xmldata.h"
#include "datasource.h"

#include <QMetaType>

class QString;

/*!
 * \brief A task is a named description of a certain kind of work.
 *
 * It contains a unique id, a name and a value when it was last used (this feature is not
 * taken advantage of yet). The name is the value that can be entered by the user when
 * starting or stoppig a task.
 *
 * This class provides static helper methods to load a specific `Task` instance from the
 * DOM, find a task by name or even get a complete list of all available tasks.
 */
class Task : public XmlData
{
public:
    /*!
     * Creates a new `Task` without any values.
     */
    Task();

    /*!
     * Creates a new `Task` with a data source and a DOM node.
     */
    Task(DataSource p_dataSource, const QDomElement& p_node);

    /*!
     * Creates a new instance with a data source, a name and the last-used date.
     */
    Task(DataSource p_dataSource, const QString& p_name, const QDateTime& p_lastUsed);

    /*!
     * Creates a new instance with a data source, an id, a name and the last-used date.
     */
    Task(DataSource p_dataSource, int p_id, const QString& p_name, 
         const QDateTime& p_lastUsed);

    /*!
     * Copies the values from another `Task` instance. This is not a deep copy. In the end
     * both instances reference the same data and changes in one will appear on the other
     * as well.
     */
    Task(const Task& p_other);

    /*!
     * \return
     * Returns the database id of the task or `Task::invalidId` if the task is yet to be
     * saved to database.
     */
    int id() const;

    /*!
     * Set a new database `id`.
     */
    void setId(int p_id);

    /*!
     * \return
     * Returns the name of the task.
     */
    QString name() const;

    /*!
     * Set a new `name` for the task.
     */
    void setName(const QString& p_name);

    /*!
     * \return
     * Returns the last-used date.
     */
    QDateTime lastUsed() const;

    /*!
     * Set a new `lastUsed` date.
     */
    void setLastUsed(const QDateTime& p_lastUsed);

protected:
    QString elementName() const;
};

/*! 
 * For use in a QVariant.
 *
 * \see TaskListModel
 */
Q_DECLARE_METATYPE(Task)

#endif // TASK_H
