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
    Task(QDomDocument* dataSource);

    /*!
     * Creates a new `Task` with a data source and a DOM node.
     */
    Task(QDomDocument* dataSource, QDomElement node);

    /*!
     * Creates a new instance with a data source, a name and the last-used date.
     */
    Task(QDomDocument* dataSource, QString name, QDate lastUsed);

    /*!
     * Creates a new instance with a data source, an id, a name and the last-used date.
     */
    Task(QDomDocument* dataSource, int id, QString name, QDate lastUsed);

    /*!
     * Copies the values from another `Task` instance. This is not a deep copy. In the end
     * both instances reference the same data and changes in one will appear on the other
     * as well.
     */
    Task(const Task& other);

    /*!
     * \return
     * Returns the database id of the task or `Task::invalidId` if the task is yet to be
     * saved to database.
     */
    int id() const;

    /*!
     * Set a new database `id`.
     */
    void setId(int id);

    /*!
     * \return
     * Returns the name of the task.
     */
    QString name() const;

    /*!
     * Set a new `name` for the task.
     */
    void setName(const QString& name);

    /*!
     * \return
     * Returns the last-used date.
     */
    QDate lastUsed() const;

    /*!
     * Set a new `lastUsed` date.
     */
    void setLastUsed(const QDate& lastUsed);

    /*!
     * Construct a `Task` instance from a DOM node.
     *
     * \param node
     * The <item> DOM node that contains the values to read.
     *
     * \param dataSource
     * The data source that is passed to the new `Task` instance.
     *
     * \return
     * Always returns a new instance based on the values provided.
     */
    static Task fromDomNode(QDomElement node, QDomDocument* dataSource);

    /*!
     * Get a specific `Task` instance from the database based on its id.
     *
     * \param id
     * The database id of the task.
     *
     * \param dataSource
     * The database that is searched for the `id`. This value is also passed to the new
     * `Task` instance.
     *
     * \return
     * If the `id` doesn't exist then a null-Task is returned, otherwise a valid new
     * instance is returned.
     */
    static Task get(int id, QDomDocument* dataSource);

    /*!
     * Works similar to `Task::get(int, QDomDocument*)` but searches by name rather than
     * database id.
     *
     * \param name
     * The exact name of the task.
     *
     * \param dataSource
     * The database that is searched for the `name`. This value is also passed to the new
     * `Task` instance.
     *
     * \return
     * If the `name` doesn't exist then a null-Task is returned, otherwise a valid new
     * instance is returned.
     */
    static Task findByName(QString name, QDomDocument* dataSource);

    /*!
     * Fetches a list of all available tasks from the database.
     *
     * \param dataSource
     * The database that contains the list of tasks.
     *
     * \return
     * If no tasks exist in the database then an empty list is returned, otherwise the
     * list contains all tasks that could be found.
     */
    static QList<Task> list(QDomDocument* dataSource);

    /*!
     * Get the number of tasks in the database.
     *
     * \param dataSource
     * The database that contains the list of tasks.
     *
     * \return
     * Returns the number of tasks in the database.
     */
    static int count(QDomDocument* dataSource);

private:
    /*!
     * Creates a new DOM node with the values provided and stores the result in
     * `XmlData::m_node`.
     */
    void createNode(int id, QString name, QDate lastUsed);

    /*!
     * Extends the base class' implementation with some logic tailored to the own data
     * types that are expected to be read from `XmlData::m_node`.
     */
    QVariant attributeValue(QString name) const;

    /*!
     * Generic helper that searches tasks based on the predicate function that actually
     * does the check for the correct task item.
     */
    static Task findTask(QDomDocument* dataSource, std::function<bool(Task)> predicate);

    /*!
     * Convenience method that hides some of the ugly DOM API.
     */
    static QDomNodeList getTaskNodes(QDomDocument* dataSource);
};

#endif // TASK_H
