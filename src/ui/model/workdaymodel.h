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

#ifndef WORKDAYMODEL_H
#define WORKDAYMODEL_H

#include "../../model/workdaylist.h"

#include <QAbstractListModel>
#include <QDomDocument>

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
    explicit WorkDayModel(QObject* p_parent = nullptr);

    /*!
     * Set a new data source.
     *
     * \param p_dataSource
     * The new data source from which to read the values. This replaces the old one. The
     * data source is only used, ownership stays with the calling code.
     */
    void setDataSource(const QDomDocument& p_dataSource);

    /*!
     * \return
     * Returns the number of workdays in the database.
     */
    int rowCount(const QModelIndex& p_parent = QModelIndex()) const;

    /*!
     * \return
     * Returns a specific workday from the database.
     */
    QVariant data(const QModelIndex& p_index, int p_role = Qt::DisplayRole) const;

private:
    QDomDocument m_dataSource;
    WorkDayList  m_workDayList;

};

#endif // WORKDAYMODEL_H
