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

#ifndef WORKTASKMODEL_H
#define WORKTASKMODEL_H

#include "../worktask.h"
#include "../worktime.h"

#include <QAbstractItemModel>

/*!
 * Table model to edit the start and stop times of `WorkTask` items.
 */
class WorkTaskModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the model.
     *
     * \param p_parent
     * The parent of the model.
     */
    explicit WorkTaskModel(QObject* p_parent = nullptr);

    /*!
     * Set the model's new work tasks. This replaces the previous work tasks.
     */
    void setWorkTask(const WorkTask& p_workTask);

    /*!
     * \return
     * Returns the number of available work task times.
     */
    int rowCount(const QModelIndex& p_parent) const;

    /*!
     * \return
     * Always returns the number 2. There are always two columns in the model, the start
     * and the stop time.
     */
    int columnCount(const QModelIndex& p_parent) const;

    /*!
     * \return
     * Returns "Start" for the first column and "Stop" for the second column in horizontal
     * orientation. No valid values are returned for vertical orientation. The role is
     * ignored, header are always the same for every role.
     */
    QVariant headerData(int p_section, Qt::Orientation p_orientation,
                        int p_role = Qt::DisplayRole) const;

    /*!
     * \return
     * Returns the start or stop time for the work task at the given position.
     */
    QVariant data(const QModelIndex& p_index, int p_role = Qt::DisplayRole) const;

    /*!
     * Set a new `QDateTime` for a specific work task.
     */
    bool setData(const QModelIndex& p_index, const QVariant& p_value,
                 int p_role = Qt::EditRole);

    /*!
     * \return
     * Returns `QAbstractItemModel::flags() | Qt::ItemIsEditable`.
     */
    Qt::ItemFlags flags(const QModelIndex& p_index) const;

    /*!
     * \see QAbstractItemModel::index(int, int, const QModelIndex&) const
     */
    QModelIndex index(int p_row, int p_column,
                      const QModelIndex& p_parent = QModelIndex()) const;

    /*!
     * \return
     * Always returns an invalid index. None of the items have a parent.
     */
    QModelIndex parent(const QModelIndex& p_child) const;
    
    /*!
     * Removes all the times that are specified in the list of indexes. If both times, 
     * start and stop, of one work-time are to be removed then the whole work-time item is 
     * removed.
     * 
     * \param p_indexes
     * The individual selected times. If this list is empty then no action is performed.
     */
    void removeTimes(const QModelIndexList& p_indexes);
    
public slots:
    /*!
     * Creates a work-time instance and appends it to the work-task.
     */
    void appendTime();

private:
    WorkTask        m_workTask;
    QList<WorkTime> m_workTimes;

};

#endif // WORKTASKMODEL_H
