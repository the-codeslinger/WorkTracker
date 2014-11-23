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

#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include "../../model/tasklist.h"

#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>
#include <QDomDocument>

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
    TaskListModel(const QDomDocument& dataSource, QObject* parent = nullptr);

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
    /*!
     * The data source that has been passed via the constructor.
     */
    QDomDocument m_dataSource;
    TaskList     m_taskList;

};

#endif // TASKLISTMODEL_H
