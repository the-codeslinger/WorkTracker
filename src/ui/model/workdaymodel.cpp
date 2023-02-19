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

#include "workdaymodel.h"
#include "../../model/workday.h"

#include <algorithm>

WorkDayModel::WorkDayModel(QObject* p_parent)
    : QAbstractListModel{p_parent}
{
}

void
WorkDayModel::setDataSource(DataSource p_dataSource)
{
    beginResetModel();
    m_dataSource = p_dataSource;
    m_workDayList.setDataSource(std::move(p_dataSource));
    endResetModel();
}

int
WorkDayModel::rowCount(const QModelIndex& /* ignored */) const
{
    if (!m_dataSource.isNull()) {
        return m_workDayList.size();
    }

    return 0;
}

QVariant
WorkDayModel::data(const QModelIndex& p_index, int p_role) const
{
    if (!p_index.isValid()) {
        return QVariant{};
    }

    if (Qt::DisplayRole == p_role || Qt::EditRole == p_role) {
        // Descending order means treating the last item in the tree as "0"
        auto reversedIndex = rowCount() - 1 - p_index.row();
        auto workday = m_workDayList.at(reversedIndex);
        if (workday.isNull()) {
            return QVariant{};
        }

        auto value = QVariant{};
        value.setValue(workday);
        return value;
    }

    return QVariant{};
}
