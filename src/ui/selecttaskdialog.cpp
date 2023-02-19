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

#include "selecttaskdialog.h"
#include "ui_selecttaskdialog.h"
#include "model/tasklistmodel.h"

#include <QCompleter>
#include <QPushButton>

#include <algorithm>

SelectTaskDialog::SelectTaskDialog(DataSource dataSource, QWidget* parent)
    : QDialog{parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                      | Qt::WindowCloseButtonHint}
    , ui{new Ui::SelectTaskDialog}
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    
    auto* completer = new QCompleter{this};
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setModel(new TaskListModel{std::move(dataSource), this});
    ui->lineEdit->setCompleter(completer);
    
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &SelectTaskDialog::textChanged);
}

SelectTaskDialog::~SelectTaskDialog()
{
    delete ui;
}

QString 
SelectTaskDialog::taskName() const
{
    return ui->lineEdit->text();
}

void 
SelectTaskDialog::textChanged(const QString& text)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(text.isEmpty());
}
