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

#include "tasklistmodel.h"
#include "../../model/task.h"

#include <QVariant>
#include <QDate>

#include <algorithm>

TaskListModel::TaskListModel(DataSource dataSource, QObject* parent)
    : QAbstractListModel{parent}
{
    auto taskList = TaskList{dataSource};
    m_taskList = taskList.tasksSortedByLastUsed();
}

int
TaskListModel::rowCount(const QModelIndex&) const
{
    // We don't have child-parent relationships, thus we can omit the parameter
    return m_taskList.size();
}

QVariant
TaskListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant{};
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto item = m_taskList.at(index.row());
        if (item.isNull()) {
            return QVariant{};
        }

        return QVariant{item.name()};
    }

    return QVariant{};
}


Qt::ItemFlags
TaskListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant
TaskListModel::headerData(int , Qt::Orientation, int) const
{
    return QVariant{"Tasks"};
}

void
TaskListModel::itemAppended()
{
    auto position = rowCount();
    beginInsertRows(QModelIndex{}, position, position + 1);
    endInsertRows();
}

void 
TaskListModel::beginAddTask()
{
    beginResetModel();
}

void 
TaskListModel::endAddTask()
{
    endResetModel();
}
