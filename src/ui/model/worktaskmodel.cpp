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

#include "worktaskmodel.h"

#include <QDateTime>

WorkTaskModel::WorkTaskModel(QObject* p_parent)
    : QAbstractItemModel{p_parent}
{
}

void
WorkTaskModel::setWorkTask(const WorkTask& p_workTask)
{
    beginResetModel();
    m_workTask  = p_workTask;
    m_workTimes = p_workTask.times();
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
        return QVariant{};
    }

    if (0 == p_section) {
        return tr("Start");
    }

    if (1 == p_section) {
        return tr("Stop");
    }

    // Practically impossible to get here
    return QVariant{};
}

QVariant
WorkTaskModel::data(const QModelIndex& p_index, int p_role) const
{
    if (!p_index.isValid() || p_index.row() >= m_workTimes.count()
            || p_index.column() >= 2) {
        return QVariant{};
    }

    if (Qt::DisplayRole == p_role || Qt::EditRole == p_role) {
        auto wt = m_workTimes.at(p_index.row());
        
        auto timestamp = QDateTime{};
        if (0 == p_index.column()) {
            timestamp = wt.start().toLocalTime();
        }
        else {
            timestamp = wt.stop().toLocalTime();
        }
        
        if (timestamp.isNull() && Qt::EditRole == p_role) {
            timestamp = QDateTime::currentDateTime();
        }
        
        return timestamp;
    }

    return QVariant{};
}

bool
WorkTaskModel::setData(const QModelIndex& p_index, const QVariant& p_value, int p_role)
{
    if (!p_index.isValid() || p_index.row() >= m_workTimes.count()
            || p_index.column() >= 2) {
        return false;
    }

    if (Qt::EditRole == p_role && p_value.canConvert<QDateTime>()) {
        auto wt = m_workTimes.at(p_index.row());
        auto dt = qvariant_cast<QDateTime>(p_value);

        if (0 == p_index.column()) {
            wt.setStart(dt.toUTC());
        }
        else {
            wt.setStop(dt.toUTC());
        }

        emit dataChanged(p_index, p_index, { p_role });
        return true;
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
        return QModelIndex{};
    }

    return this->createIndex(p_row, p_column);
}

QModelIndex
WorkTaskModel::parent(const QModelIndex& /* ignored */) const
{
    return QModelIndex{};
}

void
WorkTaskModel::appendTime()
{
    beginInsertRows(QModelIndex{}, m_workTimes.size(), m_workTimes.size());
    
    auto workTime = WorkTime{m_workTask.dataSource(), QDateTime{}, QDateTime{}};
    m_workTask.addTime(workTime);
    m_workTimes.append(workTime);
    
    endInsertRows();
}

void 
WorkTaskModel::removeTimes(QModelIndexList p_indexes)
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
    
    // The magic now lies in detecting whether one row can be completely removed from the
    // model or if just one data value shall be set to nothing.
    auto iter = QMutableListIterator<QModelIndex>{p_indexes};
    while (iter.hasNext()) {
        auto curIndex  = iter.next();
        auto nextIndex = QModelIndex{};
        if (iter.hasNext()) {
            nextIndex = iter.peekNext();
        }
        
        if (!curIndex.isValid() || curIndex.row() >= m_workTimes.count()) {
            continue;
        }
        
        auto workTime = m_workTimes.at(curIndex.row());
        auto removeRow = false;
        
        if (nextIndex.isValid() && curIndex.row() == nextIndex.row()) {
            // Remove the whole row (= work-time DOM node)
            removeRow = true;
            // Step to the next position in the iterator, we've just processed two
            iter.next();
        }
        else {
            // Column 0 is the start time and column 1 the stop time.
            if (0 == curIndex.column()) {
                workTime.setStart(QDateTime{});
            }
            else {
                workTime.setStop(QDateTime{});
            }
            
            // If both values are null then we might as well just remove the row.
            if (workTime.start().isNull() && workTime.stop().isNull()) {
                removeRow = true;
            }
            else {
                emit dataChanged(curIndex, curIndex, { Qt::EditRole });
            }
        }
        
        if (removeRow) {
            beginRemoveRows(QModelIndex{}, curIndex.row(), curIndex.row());
            
            m_workTimes.removeAt(curIndex.row());
            workTime.clear();
            
            endRemoveRows();
        }
    }
}
