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
