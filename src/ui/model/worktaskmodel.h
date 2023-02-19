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

#ifndef WORKTASKMODEL_H
#define WORKTASKMODEL_H

#include "../../model/worktask.h"
#include "../../model/worktime.h"

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
    void removeTimes(QModelIndexList p_indexes);
    
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
