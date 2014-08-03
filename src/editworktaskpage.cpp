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

#include "editworktaskpage.h"
#include "ui_editworktaskwidget.h"
#include "controller/editorcontroller.h"
#include "model/ui/selectedworkdaymodel.h"
#include "model/ui/worktaskmodel.h"

#include <QVBoxLayout>

EditWorkTaskPage::EditWorkTaskPage(EditorController* p_controller, QWidget* p_parent)
    : QWizardPage(p_parent)
    , ui(new Ui::EditWorktaskWidget)
    , m_controller(p_controller)
{
    ui->setupUi(this);
    ui->tasksListView->setModel(new SelectedWorkDayModel(ui->tasksListView));
    ui->taskTimesTableView->setModel(new WorkTaskModel(ui->taskTimesTableView));
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);

    setTitle(tr("Edit Worktask"));
    setSubTitle(tr("Select the task you want to edit. Changes are immediately saved. You "
                   "can also go back to the previous page and select a different day to "
                   "edit."));

    connect(ui->tasksListView, SIGNAL(clicked(QModelIndex)),
            this,              SLOT(taskSelected(QModelIndex)));
}

EditWorkTaskPage::~EditWorkTaskPage()
{
    delete ui;
}

void
EditWorkTaskPage::initializePage()
{
    auto* model = qobject_cast<SelectedWorkDayModel*>(ui->tasksListView->model());
    m_controller->setModelData(model);

    // Select the first item if there is one
    if (0 < ui->tasksListView->model()->rowCount()) {
        QModelIndex index = ui->tasksListView->model()->index(0, 0);
        ui->tasksListView->selectionModel()->select(index, QItemSelectionModel::Select);
        taskSelected(index);
    }
}

void
EditWorkTaskPage::taskSelected(const QModelIndex& p_index)
{
    if (p_index.isValid()) {
        auto* source = qobject_cast<SelectedWorkDayModel*>(ui->tasksListView->model());
        auto* destin = qobject_cast<WorkTaskModel*>(ui->taskTimesTableView->model());
        m_controller->setModelData(p_index, source, destin);
    }
}
