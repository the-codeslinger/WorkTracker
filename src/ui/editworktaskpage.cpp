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
#include "selectworkdaypage.h"
#include "selecttaskdialog.h"
#include "model/selectedworkdaymodel.h"
#include "model/worktaskmodel.h"
#include "delegate/taskdelegate.h"
#include "../controller/editorcontroller.h"

#include <QVBoxLayout>
#include <QItemSelection>
#include <QToolButton>
#include <QMessageBox>
#include <QShortcut>

EditWorkTaskPage::EditWorkTaskPage(EditorController*  controller,
                                   SelectWorkDayPage* workDayPage, 
                                   QWidget* parent)
    : QWizardPage{parent}
    , ui{new Ui::EditWorktaskWidget}
    , m_controller{controller}
    , m_workDayPage{workDayPage}
{
    ui->setupUi(this);
    
    auto* tasks = new SelectedWorkDayModel{ui->tasksListView};
    auto* times = new WorkTaskModel{ui->timesTableView};
            
    ui->tasksListView->setModel(tasks);
    ui->timesTableView->setModel(times);
    
    ui->tasksListView->setItemDelegate(new TaskDelegate{controller->dataSource(),
                                                        ui->tasksListView});
    
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 1);

    setTitle(QObject::tr("Edit Worktask"));
    setSubTitle(QObject::tr("Select the task you want to edit. Changes are immediately "
                            "saved. You can also go back to the previous page and select "
                            "a different day to edit."));
    
    auto* rmTask = new QShortcut{QKeySequence{Qt::Key_Delete}, ui->tasksListView,
                                 0, 0, Qt::WidgetWithChildrenShortcut};
    auto* rmTime = new QShortcut{QKeySequence{Qt::Key_Delete}, ui->timesTableView,
                                 0, 0, Qt::WidgetWithChildrenShortcut};
    
    connect(rmTask,               &QShortcut::activated, 
            this,                 &EditWorkTaskPage::removeTask);
    connect(rmTime,               &QShortcut::activated, 
            this,                 &EditWorkTaskPage::removeTime);
    connect(ui->addTaskButton,    &QToolButton::clicked, 
            this,                 &EditWorkTaskPage::addTask);
    connect(ui->removeTaskButton, &QToolButton::clicked, 
            this,                 &EditWorkTaskPage::removeTask);
    connect(ui->addTimeButton,    &QToolButton::clicked, 
            this,                 &EditWorkTaskPage::addTime);
    connect(ui->removeTimeButton, &QToolButton::clicked, 
            this,                 &EditWorkTaskPage::removeTime);
    connect(tasks,                &SelectedWorkDayModel::taskAlreadyExists, 
            this,                 &EditWorkTaskPage::taskAlreadyExists);
    
    // Model validation.
    connect(tasks,                &QAbstractItemModel::dataChanged,
            this,                 &EditWorkTaskPage::validateModel);
    connect(times,                &QAbstractItemModel::dataChanged,
            this,                 &EditWorkTaskPage::validateModel);
    connect(tasks,                &QAbstractItemModel::rowsInserted,
            this,                 &EditWorkTaskPage::validateModel);
    connect(times,                &QAbstractItemModel::rowsInserted,
            this,                 &EditWorkTaskPage::validateModel);
    connect(tasks,                &QAbstractItemModel::rowsRemoved,
            this,                 &EditWorkTaskPage::validateModel);
    connect(times,                &QAbstractItemModel::rowsRemoved,
            this,                 &EditWorkTaskPage::validateModel);
    connect(times,                &QAbstractItemModel::columnsRemoved,
            this,                 &EditWorkTaskPage::validateModel);
    connect(controller,           &EditorController::validationError,
            this,                 &EditWorkTaskPage::validationError);
    connect(controller,           &EditorController::validationSuccess,
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
    
    auto value = m_workDayPage->selectedItem();
    if (value.canConvert<WorkDay>()) {
        model->setWorkDay(qvariant_cast<WorkDay>(value));
    }

    // Select the first item if there is one
    if (0 < model->rowCount(QModelIndex{})) {
        auto index = model->index(0, 0);
        ui->tasksListView->selectionModel()->select(index, QItemSelectionModel::Select);
    }
}

void 
EditWorkTaskPage::addTask()
{
    SelectTaskDialog dlg{m_controller->dataSource()};
    if (QDialog::Accepted == dlg.exec()) {
        auto* view = ui->tasksListView;
        auto* model = qobject_cast<SelectedWorkDayModel*>(view->model());
        model->appendTask(dlg.taskName());
        
        auto* selectionModel = view->selectionModel();
        auto  newItemIndex   = model->index(model->rowCount(QModelIndex{}) - 1);
        
        selectionModel->clearSelection();
        selectionModel->select(newItemIndex, QItemSelectionModel::Select);
        
        view->setFocus();
    }
}

void 
EditWorkTaskPage::removeTask()
{
    auto  indexes = selectedTasks();
    auto* view    = ui->tasksListView;
    auto* model   = qobject_cast<SelectedWorkDayModel*>(view->model());
    
    if (0 == indexes.size()) {
        return;
    }
    
    auto question = QString{};
    if (1 == indexes.size()) {
        auto workTask = model->workTask(indexes.at(0));
        if (workTask.isNull()) {
            return;
        }
        
        question = tr("Are you sure you want to delete the work-task \"%1\" \n"
                      "and all of its recorded times?").arg(workTask.task().name());
    }
    else {
        question = tr("Are you sure you want to delete the selected %1 \n"
                      "work-tasks and all of their recorded times?").arg(indexes.size());
    }
    
    int result = QMessageBox::question(this, tr("Delete Task"), question, 
                                       QMessageBox::Yes, QMessageBox::No);
    
    if (QMessageBox::Yes == result) {
        model->removeTasks(indexes);
    }
}

void 
EditWorkTaskPage::addTime()
{
    auto* view = ui->timesTableView;
    auto* model = qobject_cast<WorkTaskModel*>(view->model());
    model->appendTime();
    
    auto* selectionModel = view->selectionModel();
    auto  newItemIndex   = model->index(model->rowCount(QModelIndex{}) - 1, 0);
    
    selectionModel->clearSelection();
    selectionModel->select(newItemIndex, QItemSelectionModel::Select);
    
    view->setFocus();
}

void 
EditWorkTaskPage::removeTime()
{
    auto indexes = selectedTimes();
    if (!indexes.isEmpty()) {
        auto result = QMessageBox::question(
                    nullptr/*m_editWorkTaskPage*/, tr("Delete Time"), 
                    tr("Are you sure you want to delete %1 recorded times?")
                      .arg(indexes.size()), 
                    QMessageBox::Yes, QMessageBox::No);
        
        if (QMessageBox::Yes == result) {
            auto* view  = ui->timesTableView;
            auto* model = qobject_cast<WorkTaskModel*>(view->model());
            model->removeTimes(indexes);
        }
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

void
EditWorkTaskPage::taskSelected(const QItemSelection& selection)
{
    auto indexes = selection.indexes();
    if (!indexes.isEmpty()) {
        if (1 == indexes.size()) {
            auto index = indexes.at(0);
            if (index.isValid()) {
                auto* src = qobject_cast<SelectedWorkDayModel*>(ui->tasksListView->model());
                auto* dst = qobject_cast<WorkTaskModel*>(ui->timesTableView->model());
                dst->setWorkTask(src->workTask(index));
            }
        }
    }
    
    ui->removeTaskButton->setDisabled(indexes.isEmpty());
    ui->removeTimeButton->setDisabled(indexes.isEmpty());
    ui->addTimeButton->setDisabled(indexes.isEmpty());
}

void 
EditWorkTaskPage::timeSelected(const QItemSelection& selection)
{
    auto indexes = selection.indexes();
    ui->removeTimeButton->setDisabled(indexes.isEmpty());
}

void 
EditWorkTaskPage::changeEvent(QEvent* event)
{
    if (QEvent::LanguageChange == event->type()) {
        ui->retranslateUi(this);
    }
    
    QWizardPage::changeEvent(event);
}

void 
EditWorkTaskPage::taskAlreadyExists(const QString& name)
{
    QMessageBox::information(
                this, tr("Task already exists"), 
                tr("There is already a task in use with the name \"%1\".").arg(name), 
                QMessageBox::Ok);
}

void 
EditWorkTaskPage::validationError(const QString& error)
{
    ui->errorLabel->setText(error);
}

void 
EditWorkTaskPage::validationSuccess()
{
    ui->errorLabel->setText("");
}

void 
EditWorkTaskPage::validateModel()
{
    auto value = m_workDayPage->selectedItem();
    if (value.canConvert<WorkDay>()) {
        m_controller->validateModel(qvariant_cast<WorkDay>(value));
    }
}
