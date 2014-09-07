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

#include "selectedworkdaymodel.h"

SelectedWorkDayModel::SelectedWorkDayModel(QObject* p_parent)
    : QAbstractListModel(p_parent)
{
}

int
SelectedWorkDayModel::rowCount(const QModelIndex& /* ignored */) const
{
    return m_tasks.size();
}

QVariant
SelectedWorkDayModel::data(const QModelIndex& p_index, int p_role) const
{
    if (!p_index.isValid() || p_index.row() >= m_tasks.count()) {
        return QVariant();
    }

    if (Qt::DisplayRole == p_role || Qt::EditRole == p_role) {
        return QVariant(m_tasks.at(p_index.row()).name());
    }

    return QVariant();
}

bool 
SelectedWorkDayModel::setData(const QModelIndex& p_index, const QVariant& p_value, 
                              int p_role)
{
    if (!p_index.isValid() || p_index.row() >= m_tasks.count()) {
        return false;
    }
    
    if (Qt::EditRole == p_role) {
        Task task = m_tasks.at(p_index.row());
        QString name = p_value.toString();
        if (!name.isEmpty()) {
            task.setName(name);
            return true;
        }
    }
    
    return false;
}

Qt::ItemFlags 
SelectedWorkDayModel::flags(const QModelIndex& p_index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(p_index);
    flags |= Qt::ItemIsEditable;
    return flags;
}

void
SelectedWorkDayModel::setWorkDay(const WorkDay& p_workday)
{
    beginResetModel();

    m_workday = p_workday;
    m_tasks   = m_workday.distinctTasks();

    endResetModel();
}

QList<WorkTask>
SelectedWorkDayModel::workTasks(const QModelIndex& p_index) const
{
    if (!p_index.isValid() || p_index.row() >= m_tasks.count()) {
        return QList<WorkTask>();
    }

    return m_workday.workTasks(m_tasks.at(p_index.row()));
}
