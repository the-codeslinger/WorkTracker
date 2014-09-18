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
    QMutableListIterator<QModelIndex> iter(p_indexes);
    while (iter.hasNext()) {
        QModelIndex curIndex  = iter.next();
        QModelIndex nextIndex;
        if (iter.hasNext()) {
            nextIndex = iter.peekNext();
        }
        
        if (!curIndex.isValid() || curIndex.row() >= m_workTimes.count()) {
            continue;
        }
        
        WorkTime workTime = m_workTimes.at(curIndex.row());
        bool removeRow = false;
        
        if (nextIndex.isValid() && curIndex.row() == nextIndex.row()) {
            // Remove the whole row (= work-time DOM node)
            removeRow = true;
            // Step to the next position in the iterator, we've just processed two
            iter.next();
        }
        else {
            // Column 0 is the start time and column 1 the stop time.
            if (0 == curIndex.column()) {
                workTime.setStart(QDateTime());
            }
            else {
                workTime.setStop(QDateTime());
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
            beginRemoveRows(QModelIndex(), curIndex.row(), curIndex.row());
            
            m_workTimes.removeAt(curIndex.row());
            workTime.remove();
            
            endRemoveRows();
        }
    }
}
