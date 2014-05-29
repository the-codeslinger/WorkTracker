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
     * Add a completed `task` to the list of already completed tasks.
     */
    void addTask(WorkTask task);

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
     * Helper that rounds `number` to two decimals for use in the summary.
     */
    float roundTwoDecimals(float number) const;

    /*!
     * Create a new `WorkDay` from the DOM node.
     */
    static WorkDay fromDomNode(QDomElement node, QDomDocument* dataSource);
};

#endif // WORKDAY_H
