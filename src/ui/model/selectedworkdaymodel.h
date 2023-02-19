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

#ifndef SELECTEDWORKDAYMODEL_H
#define SELECTEDWORKDAYMODEL_H

#include "../../model/workday.h"

#include <QAbstractListModel>

/*!
 * Read-only model for all the distinct work tasks of a specific workday.
 */
class SelectedWorkDayModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the model.
     *
     * \param p_parent
     * Parent of the model.
     */
    explicit SelectedWorkDayModel(QObject* p_parent = nullptr);

    /*!
     * \return
     * Returns the distinct number of worktasks.
     */
    int rowCount(const QModelIndex& p_parent) const;

    /*!
     * \return
     * Returns the name of the work task at the given position if the index is valid.
     */
    QVariant data(const QModelIndex& p_index, int p_role = Qt::DisplayRole) const;
    
    /*!
     * Set name of a specific task.
     * 
     * \param p_index
     * The position of the task in the model.
     * 
     * \param p_value
     * The new name of the task.
     * 
     * \param p_role
     * The role for which to edit the task. Only `Qt::EditRole` is handled.
     * 
     * \return 
     * Returns `true` if the name was changed or `false` otherwise.
     */
    bool setData(const QModelIndex& p_index, const QVariant& p_value, 
                 int p_role = Qt::EditRole);
    
    /*!
     * Removes the tasks and all of its recorded times from the list that are mentioned in
     * the list of indexes. This is permanent and cannot be undone.
     */
    void removeTasks(QModelIndexList p_indexes);

    /*!
     * \return 
     * Returns the supported flags for the item at the specified index.
     */
    Qt::ItemFlags flags(const QModelIndex& p_index) const;
    
    /*!
     * Set a new workday for the model. This replaces the old one.
     */
    void setWorkDay(const WorkDay& p_workday);

    /*!
     * \return
     * Returns the list of work task items for the selected index.
     */
    WorkTask workTask(const QModelIndex& p_index) const;
    
signals:
    /*!
     * Emitted if `SelectedWorkDayModel::appendTask(const QString&)` is called with the
     * name of a task for which a work-task already exists.
     * 
     * \param p_name
     * The name of the task that caused the conflict.
     */
    void taskAlreadyExists(const QString& p_name);
    
public slots:
    /*!
     * Creates a task with a name and appends it to the list of existing tasks. If 
     * `p_name` is null then no action is performed.
     */
    void appendTask(const QString& p_name);
    

private:
    WorkDay m_workday;
};

#endif // SELECTEDWORKDAYMODEL_H
