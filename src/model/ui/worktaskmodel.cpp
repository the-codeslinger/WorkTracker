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

#include "worktaskmodel.h"

WorkTaskModel::WorkTaskModel(QObject* p_parent)
    : QAbstractItemModel(p_parent)
{
}

void
WorkTaskModel::setWorkTask(const WorkTask& p_workTask)
{
    beginResetModel();
    m_workTask  = p_workTask;
    m_workTimes = p_workTask.workTimes();
    endResetModel();
}

int
WorkTaskModel::rowCount(const QModelIndex& /* ignored */) const
{
    return m_workTimes.size();
}

int
WorkTaskModel::columnCount(const QModelIndex& /* ignored */) const
{
    return 2;
}

QVariant
WorkTaskModel::headerData(int p_section, Qt::Orientation p_orientation,
                          int p_role) const
{
    // Vertical orientation or an index out of bounds for an "array of size 2" return
    // nothing (only two columns supported)
    if (Qt::DisplayRole != p_role || Qt::Vertical == p_orientation || 2 <= p_section) {
        return QVariant();
    }

    if (0 == p_section) {
        return tr("Start");
    }

    if (1 == p_section) {
        return tr("Stop");
    }

    // Practically impossible to get here
    return QVariant();
}

QVariant
WorkTaskModel::data(const QModelIndex& p_index, int p_role) const
{
    if (!p_index.isValid() || p_index.row() >= m_workTimes.count()
            || p_index.column() >= 2) {
        return QVariant();
    }

    if (Qt::DisplayRole == p_role || Qt::EditRole == p_role) {
        WorkTime wt = m_workTimes.at(p_index.row());
        if (0 == p_index.column()) {
            return wt.start().toLocalTime();
        }
        else {
            return wt.stop().toLocalTime();
        }
    }

    return QVariant();
}

bool
WorkTaskModel::setData(const QModelIndex& p_index, const QVariant& p_value, int p_role)
{
    if (!p_index.isValid() || p_index.row() >= m_workTimes.count()
            || p_index.column() >= 2) {
        return false;
    }

    if (Qt::EditRole == p_role && p_value.canConvert<QDateTime>()) {
        WorkTime  wt = m_workTimes.at(p_index.row());
        QDateTime dt = qvariant_cast<QDateTime>(p_value);

        if (0 == p_index.column()) {
            wt.setStart(dt.toUTC());
        }
        else {
            wt.setStop(dt.toUTC());
        }

        return true;
        emit dataChanged(p_index, p_index, { Qt::EditRole });
    }

    return false;
}

Qt::ItemFlags
WorkTaskModel::flags(const QModelIndex& p_index) const
{
    return QAbstractItemModel::flags(p_index) | Qt::ItemIsEditable;
}

QModelIndex
WorkTaskModel::index(int p_row, int p_column, const QModelIndex& /* ignored */) const
{
    if (p_row >= m_workTimes.count() || p_column >= 2) {
        return QModelIndex();
    }

    return this->createIndex(p_row, p_column);
}

QModelIndex
WorkTaskModel::parent(const QModelIndex& /* ignored */) const
{
    return QModelIndex();
}

void
WorkTaskModel::appendTime()
{
    beginInsertRows(QModelIndex(), m_workTimes.size(), m_workTimes.size());
    
    WorkTime workTime(m_workTask.dataSource());
    m_workTask.addTime(workTime);
    m_workTimes.append(workTime);
    
    endInsertRows();
}

void 
WorkTaskModel::removeTimes(const QModelIndexList& p_indexes)
{
    if (p_indexes.isEmpty()) {
        return;
    }
}
