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
