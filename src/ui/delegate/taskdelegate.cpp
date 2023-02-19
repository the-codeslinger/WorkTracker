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

#include "taskdelegate.h"
#include "../model/tasklistmodel.h"

#include <QCompleter>
#include <QLineEdit>

TaskDelegate::TaskDelegate(DataSource p_dataSource, QObject* p_parent)
    : QStyledItemDelegate{p_parent}
    , m_dataSource{std::move(p_dataSource)}
{
}

QWidget* 
TaskDelegate::createEditor(QWidget* p_parent, const QStyleOptionViewItem& /* p_option */, 
                           const QModelIndex& p_index) const
{
    if (!p_index.isValid()) {
        return nullptr;
    }
    
    auto* lineEdit  = new QLineEdit(p_parent);
    auto* completer = new QCompleter(lineEdit);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setModel(new TaskListModel(m_dataSource, completer));
    lineEdit->setCompleter(completer);
    return lineEdit;
}
