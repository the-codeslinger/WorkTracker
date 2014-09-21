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
#include "model/delegate/taskdelegate.h"

#include <QVBoxLayout>
#include <QItemSelection>
#include <QToolButton>
#include <QMessageBox>
#include <QShortcut>

EditWorkTaskPage::EditWorkTaskPage(EditorController* p_controller, QWidget* p_parent)
    : QWizardPage(p_parent)
    , ui(new Ui::EditWorktaskWidget)
    , m_controller(p_controller)
{
    ui->setupUi(this);
    
    SelectedWorkDayModel* tasks = new SelectedWorkDayModel(ui->tasksListView);
    WorkTaskModel* times = new WorkTaskModel(ui->timesTableView);
            
    ui->tasksListView->setModel(tasks);
    ui->timesTableView->setModel(times);
    
    ui->tasksListView->setItemDelegate(new TaskDelegate(p_controller->dataSource(),
                                                        ui->tasksListView));
    
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 1);

    setTitle(tr("Edit Worktask"));
    setSubTitle(tr("Select the task you want to edit. Changes are immediately saved. You "
                   "can also go back to the previous page and select a different day to "
                   "edit."));
    
    QShortcut* rmTask = new QShortcut(QKeySequence(Qt::Key_Delete), ui->tasksListView, 
                                      0, 0, Qt::WidgetWithChildrenShortcut);
    QShortcut* rmTime = new QShortcut(QKeySequence(Qt::Key_Delete), ui->timesTableView, 
                                      0, 0, Qt::WidgetWithChildrenShortcut);
    
    connect(rmTask,               &QShortcut::activated, 
            p_controller,         &EditorController::removeTask);
    connect(rmTime,               &QShortcut::activated, 
            p_controller,         &EditorController::removeTime);
    connect(ui->addTaskButton,    &QToolButton::clicked, 
            p_controller,         &EditorController::addTask);
    connect(ui->removeTaskButton, &QToolButton::clicked, 
            p_controller,         &EditorController::removeTask);
    connect(ui->addTimeButton,    &QToolButton::clicked, 
            p_controller,         &EditorController::addTime);
    connect(ui->removeTimeButton, &QToolButton::clicked, 
            p_controller,         &EditorController::removeTime);
    connect(tasks,                &SelectedWorkDayModel::taskAlreadyExists, 
            this,                 &EditWorkTaskPage::taskAlreadyExists);
    
    // Model validation.
    connect(tasks,                &QAbstractItemModel::dataChanged,
            p_controller,         &EditorController::validateModel);
    connect(times,                &QAbstractItemModel::dataChanged,
            p_controller,         &EditorController::validateModel);
    connect(tasks,                &QAbstractItemModel::rowsInserted,
            p_controller,         &EditorController::validateModel);
    connect(times,                &QAbstractItemModel::rowsInserted,
            p_controller,         &EditorController::validateModel);
    connect(tasks,                &QAbstractItemModel::rowsRemoved,
            p_controller,         &EditorController::validateModel);
    connect(times,                &QAbstractItemModel::rowsRemoved,
            p_controller,         &EditorController::validateModel);
    connect(p_controller,         &EditorController::validationError,
            this,                 &EditWorkTaskPage::validationError);
    connect(p_controller,         &EditorController::validationSuccess,
            this,                 &EditWorkTaskPage::validationSuccess);
    
    connect(ui->tasksListView->selectionModel(), 
                  SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(taskSelected(QItemSelection)));
    
    connect(ui->timesTableView->selectionModel(), 
                  SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(timeSelected(QItemSelection)));
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
    }
}

QModelIndexList 
EditWorkTaskPage::selectedTasks() const
{
    return ui->tasksListView->selectionModel()->selectedIndexes();
}

QModelIndexList 
EditWorkTaskPage::selectedTimes() const
{
    return ui->timesTableView->selectionModel()->selectedIndexes();
}

QListView* 
EditWorkTaskPage::workTasksView() const
{
    return ui->tasksListView;
}

QTableView* 
EditWorkTaskPage::workTimesView() const
{
    return ui->timesTableView;
}

void
EditWorkTaskPage::taskSelected(const QItemSelection& p_selection)
{
    QModelIndexList indexes = p_selection.indexes();
    if (!indexes.isEmpty()) {
        // Only single selection allowed
        QModelIndex index = indexes.at(0);
        if (index.isValid()) {
            auto* source = qobject_cast<SelectedWorkDayModel*>(ui->tasksListView->model());
            auto* destin = qobject_cast<WorkTaskModel*>(ui->timesTableView->model());
            m_controller->setModelData(index, source, destin);
        }
    }
    
    ui->removeTaskButton->setDisabled(indexes.isEmpty());
}

void 
EditWorkTaskPage::timeSelected(const QItemSelection& p_selection)
{
    QModelIndexList indexes = p_selection.indexes();
    ui->removeTimeButton->setDisabled(indexes.isEmpty());
}

void 
EditWorkTaskPage::changeEvent(QEvent* p_event)
{
    if (QEvent::LanguageChange == p_event->type()) {
        ui->retranslateUi(this);
    }
    
    QWizardPage::changeEvent(p_event);
}

void 
EditWorkTaskPage::taskAlreadyExists(const QString& p_name)
{
    QMessageBox::information(
                this, tr("Task already exists"), 
                tr("There is already a task in use with the name \"%1\".").arg(p_name), 
                QMessageBox::Ok);
}

void 
EditWorkTaskPage::validationError(const QString& p_error)
{
    ui->errorLabel->setText(p_error);
}

void 
EditWorkTaskPage::validationSuccess()
{
    ui->errorLabel->setText("");
}
