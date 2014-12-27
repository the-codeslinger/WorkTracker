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
#include "../../model/task.h"
#include "../../model/tasklist.h"

#include <QDate>

#include <algorithm>

SelectedWorkDayModel::SelectedWorkDayModel(QObject* p_parent)
    : QAbstractListModel(p_parent)
{
}

int
SelectedWorkDayModel::rowCount(const QModelIndex& /* ignored */) const
{
    return m_workday.countWorkTasks();
}

QVariant
SelectedWorkDayModel::data(const QModelIndex& p_index, int p_role) const
{
    if (!p_index.isValid() || p_index.row() >= m_workday.countWorkTasks()) {
        return QVariant();
    }

    if (Qt::DisplayRole == p_role || Qt::EditRole == p_role) {
        auto wt = m_workday.at(p_index.row());
        return QVariant(wt.task().name());
    }

    return QVariant();
}

bool 
SelectedWorkDayModel::setData(const QModelIndex& p_index, const QVariant& p_value, 
                              int p_role)
{
    if (!p_index.isValid() || p_index.row() >= m_workday.countWorkTasks()) {
        return false;
    }
    
    if (Qt::EditRole == p_role) {
        QString name = p_value.toString();
        if (!name.isEmpty()) {
            auto dataSource = m_workday.dataSource();
            auto tlist      = TaskList{dataSource};
            auto task       = tlist.find(name);
            if (task.isNull()) {
                task = Task{dataSource, name, QDate::currentDate()};
                tlist.add(task);
            }
            
            auto existing = m_workday.findWorkTask(task);
            auto current  = m_workday.at(p_index.row());
            
            // If the to-be-changed work-task is different from another worktask that
            // already exists with that name then we'd have two different work-tasks with
            // the same task. We don't allow that.
            if (existing.isNull() || existing == current) {
                current.setTask(task);
                
                emit dataChanged(p_index, p_index, { p_role });
                return true;
            }
            else {
                emit taskAlreadyExists(name);
                return false;
            }
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

    m_workday   = p_workday;

    endResetModel();
}

WorkTask
SelectedWorkDayModel::workTask(const QModelIndex& p_index) const
{
    if (!p_index.isValid() || p_index.row() >= m_workday.countWorkTasks()) {
        return WorkTask();
    }

    return m_workday.at(p_index.row());
}

void
SelectedWorkDayModel::appendTask(const QString& p_name)
{
    if (p_name.isEmpty()) {
        return;
    }
    
    auto dataSource = m_workday.dataSource();
    auto tlist      = TaskList{dataSource};
    auto task       = tlist.find(p_name);
    if (task.isNull()) {
        task = Task{dataSource, p_name, QDate::currentDate()};
        tlist.add(task);
    }
    
    auto workTask = m_workday.findWorkTask(task);
    if (!workTask.isNull()) {
        emit taskAlreadyExists(p_name);
        return;
    }
    
    auto count = m_workday.countWorkTasks();
    beginInsertRows(QModelIndex(), count, count);
    
    // We know nothing about this task right now, other than it exists. The setData() 
    // method has to do all the heavy lifting to figure out where it belongs once it
    // receives a name.
    m_workday.addWorkTask(WorkTask{dataSource, task});
    
    endInsertRows();
}

void
SelectedWorkDayModel::removeTasks(QModelIndexList p_indexes)
{
    if (p_indexes.isEmpty()) {
        return;
    }
    
    // first we need to sort the indexes to start with the highes row and end with the
    // lowest row. We then remove from m_workTasks starting with the highest row number
    // in order to not mess up the indexes of the lower row-numbers.
    std::sort(std::begin(p_indexes), std::end(p_indexes), 
              [](const QModelIndex& p_first, const QModelIndex& p_second) {
        return p_second.row() < p_first.row();
    });
    
    for (const QModelIndex& index : p_indexes) {
        if (!index.isValid() || index.row() >= m_workday.countWorkTasks()) {
            continue;
        }
        
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        
        auto workTask = m_workday.at(index.row());
        workTask.clear();
        
        endRemoveRows();
    }
}
