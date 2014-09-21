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

#include <QString>
#include <QDate>
#include <QDomNode>
#include <QList>
#include <QVariant>

#include <functional>

class QDomDocument;
class QDomAttr;

/*!
 * \brief A task is a named description of a certain kind of work.
 *
 * It contains a unique id, a name and a value when it was last used (this feature is not
 * taken advantage of, yet). The name is the value that can be entered by the user when
 * starting or stoppig a task.
 *
 * This class provides static helper methods to load a specific `Task` instance from the
 * DOM, find a task by name or even get a complete list of all available tasks.
 */
class Task : public XmlData
{
public:
    /*!
     * Constant value. If a task has not yet been saved to the DOM then its
     * `Task::id() const` method returns `Task::invalidId`.
     */
    static int invalidId;

    /*!
     * Creates a new `Task` without any values.
     */
    Task();

    /*!
     * Creates a new `Task` with a data source
     */
    Task(const QDomDocument& p_p_dataSource);

    /*!
     * Creates a new `Task` with a data source and a DOM node.
     */
    Task(const QDomDocument& p_dataSource, const QDomElement& p_node);

    /*!
     * Creates a new instance with a data source, a name and the last-used date.
     */
    Task(const QDomDocument& p_dataSource, const QString& p_name, 
         const QDate& p_lastUsed);

    /*!
     * Creates a new instance with a data source, an id, a name and the last-used date.
     */
    Task(const QDomDocument& p_dataSource, int p_id, const QString& p_name, 
         const QDate& p_lastUsed);

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
    QDate lastUsed() const;

    /*!
     * Set a new `lastUsed` date.
     */
    void setLastUsed(const QDate& p_lastUsed);

    /*!
     * Get a specific `Task` instance from the database based on its id.
     *
     * \param id
     * The database id of the task.
     *
     * \param p_dataSource
     * The database that is searched for the `id`. This value is also passed to the new
     * `Task` instance.
     *
     * \return
     * If the `id` doesn't exist then a null-Task is returned, otherwise a valid new
     * instance is returned.
     */
    static Task get(int p_id, const QDomDocument& p_dataSource);

    /*!
     * Works similar to `Task::get(int, const QDomDocument&)` but searches by name rather than
     * database id.
     *
     * \param name
     * The exact name of the task.
     *
     * \param p_dataSource
     * The database that is searched for the `name`. This value is also passed to the new
     * `Task` instance.
     *
     * \return
     * If the `name` doesn't exist then a null-Task is returned, otherwise a valid new
     * instance is returned.
     */
    static Task findByName(const QString& p_name, const QDomDocument& p_dataSource);

    /*!
     * Fetches a list of all available tasks from the database.
     *
     * \param p_dataSource
     * The database that contains the list of tasks.
     *
     * \return
     * If no tasks exist in the database then an empty list is returned, otherwise the
     * list contains all tasks that could be found.
     */
    static QList<Task> list(const QDomDocument& p_dataSource);

    /*!
     * Get the number of tasks in the database.
     *
     * \param p_dataSource
     * The database that contains the list of tasks.
     *
     * \return
     * Returns the number of tasks in the database.
     */
    static int count(const QDomDocument& p_dataSource);

private:
    /*!
     * Creates a new DOM node with the values provided and stores the result in
     * `XmlData::m_node`.
     */
    void createNode(int p_id, const QString& p_name, const QDate& p_lastUsed);
    
    /*!
     * Adds the task to the <tasks> DOM node.
     */
    void addToList();

    /*!
     * Generic helper that searches tasks based on the predicate function that actually
     * does the check for the correct task item.
     */
    static Task findTask(const QDomDocument&             p_dataSource, 
                         std::function<bool(Task)> p_predicate);

    /*!
     * Convenience method that hides some of the ugly DOM API.
     */
    static QDomNodeList getTaskNodes(const QDomDocument& p_dataSource);
};

#endif // TASK_H
