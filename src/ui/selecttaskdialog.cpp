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
