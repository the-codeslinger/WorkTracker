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
