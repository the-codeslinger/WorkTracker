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

#ifndef WORKDAYMODEL_H
#define WORKDAYMODEL_H

#include "../../model/datasource.h"
#include "../../model/workdaylist.h"

#include <QAbstractListModel>

/*!
 * Returns a read-only list of all the workdays in the database.
 */
class WorkDayModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * Create a new model with a data source and a parent.
     *
     * \param parent
     * The parent of the `TaskListModel`.
     */
    explicit WorkDayModel(QObject* parent = nullptr);

    /*!
     * Set a new data source.
     *
     * \param dataSource
     * The new data source from which to read the values. This replaces the old one. The
     * data source is only used, ownership stays with the calling code.
     */
    void setDataSource(DataSource dataSource);

    /*!
     * \return
     * Returns the number of workdays in the database.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    /*!
     * \return
     * Returns a specific workday from the database.
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    DataSource  m_dataSource;
    WorkDayList m_workDayList;

};

#endif // WORKDAYMODEL_H
