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

#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include "../../model/datasource.h"
#include "../../model/tasklist.h"

#include <QAbstractListModel>
#include <QModelIndex>

class QVariant;

/*!
 * Internally this uses the DOM document as data source and the `Task` class to conform
 * to the API dictated by Qt.
 */
class TaskListModel : public QAbstractListModel
{
public:
    /*!
     * Create a new model with a data source and a parent.
     *
     * \param dataSource
     * The data source is only used, ownership stays with the calling code.
     *
     * \param parent
     * The parent of the `TaskListModel`.
     */
    TaskListModel(DataSource dataSource, QObject* parent = nullptr);

    /*!
     * This method must be called if a new `Task` has been added to the database. Since
     * adding is performed outside of this model, the necessary Qt methods need to be
     * triggered explicitly from outside this model as well.
     */
    void itemAppended();

    /*!
     * \return
     * Returns the number of tasks in the database.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    /*!
     * \return
     * Gets a specific task from the database. The index corresponds to the database id.
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    /*!
     * \return
     * If `index` is valid then `QAbstractItemModel::flags(index) | Qt::ItemIsEditable` is
     * returned.
     */
    Qt::ItemFlags flags(const QModelIndex & index) const;

    /*!
     * \return
     * Returns "Tasks".
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    
public slots:
    /*!
     * Calles `beginModelReset()`. Connect this to `Task::aboutToAddTask()`.
     */
    void beginAddTask();
    
    /*!
     * Calles `endModelReset()`. Connect this to `Task::taskAdded()`.
     */
    void endAddTask();

private:
    QList<Task> m_taskList;

};

#endif // TASKLISTMODEL_H
