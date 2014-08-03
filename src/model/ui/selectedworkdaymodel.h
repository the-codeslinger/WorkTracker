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

#ifndef SELECTEDWORKDAYMODEL_H
#define SELECTEDWORKDAYMODEL_H

#include "../workday.h"

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

    /**
     * @return
     * Returns the distinct number of worktasks.
     */
    int rowCount(const QModelIndex& p_parent) const;

    /**
     * @return
     * Returns the name of the work task at the given position if the index is valid.
     */
    QVariant data(const QModelIndex& p_index, int p_role = Qt::DisplayRole) const;

    /*!
     * Set a new workday for the model. This replaces the old one.
     */
    void setWorkDay(const WorkDay& p_workday);

    /*!
     * \return
     * Returns the list of work task items for the selected index.
     */
    QList<WorkTask> workTasks(const QModelIndex& p_index) const;

private:
    WorkDay     m_workday;
    QList<Task> m_tasks;
};

#endif // SELECTEDWORKDAYMODEL_H
