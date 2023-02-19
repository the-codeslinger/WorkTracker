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

#ifndef TASKDELEGATE_H
#define TASKDELEGATE_H

#include "../../model/datasource.h"

#include <QStyledItemDelegate>

/*!
 * Provides a completer for an item in a list of tasks.
 */
class TaskDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Create a new delegate.
     *
     * \param p_dataSource
     * The database connection.
     *
     * \param p_parent
     * Parent of the delegate.
     */
    TaskDelegate(DataSource p_dataSource, QObject* p_parent = nullptr);
    
    /*!
     * Creates and returns the line-edit widget with a connected completer.
     */
    QWidget* createEditor(QWidget* p_parent, const QStyleOptionViewItem& p_option, 
                          const QModelIndex& p_index) const;
    
private:
    DataSource m_dataSource;
};

#endif // TASKDELEGATE_H
