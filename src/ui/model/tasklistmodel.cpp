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
