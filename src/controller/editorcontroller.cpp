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

#include "editorcontroller.h"
#include "../ui/selectworkdaypage.h"
#include "../ui/editworktaskpage.h"
#include "../ui/model/workdaymodel.h"
#include "../ui/model/worktaskmodel.h"
#include "../ui/model/selectedworkdaymodel.h"
#include "../ui/selecttaskdialog.h"
#include "../ui/editorwizard.h"

#include <QWizard>
#include <QListView>
#include <QTableView>
#include <QMessageBox>

EditorController::EditorController(const QDomDocument& p_dataSource, QObject* p_parent)
    : QObject(p_parent)
    , m_dataSource(p_dataSource)
{
}

void
EditorController::run()
{
    EditorWizard wizard(this);

    m_selectWorkDayPage = new SelectWorkDayPage(this, &wizard);
    m_editWorkTaskPage  = new EditWorkTaskPage(this, &wizard);

    wizard.addPage(m_selectWorkDayPage);
    wizard.addPage(m_editWorkTaskPage);
    
    connect(this,    &EditorController::validationSuccess,
            &wizard, &EditorWizard::validationSuccess);
    connect(this,    &EditorController::validationError,
            &wizard, &EditorWizard::validationError);
    connect(&wizard, &EditorWizard::finished,
            this,    &EditorController::updateActiveWorkTasks);
    
    QSize size = wizard.size();
    wizard.resize(size.width() * 1.05, size.height() * 1.05);
    wizard.exec();
}

void
EditorController::setModelData(WorkDayModel* p_model)
{
    p_model->setDataSource(m_dataSource);
}

void
EditorController::setModelData(SelectedWorkDayModel* p_model)
{
    QVariant value = m_selectWorkDayPage->selectedItem();
    if (value.canConvert<WorkDay>()) {
        p_model->setWorkDay(qvariant_cast<WorkDay>(value));
    }
}

void
EditorController::setModelData(const QModelIndex& p_index, SelectedWorkDayModel* p_source,
                               WorkTaskModel* p_destination)
{
    p_destination->setWorkTask(p_source->workTask(p_index));
}

void 
EditorController::addTask()
{
    SelectTaskDialog dlg(m_dataSource);
    if (QDialog::Accepted == dlg.exec()) {
        QListView* view = m_editWorkTaskPage->workTasksView();
        SelectedWorkDayModel* model = qobject_cast<SelectedWorkDayModel*>(view->model());
        model->appendTask(dlg.taskName());
        
        QItemSelectionModel* selectionModel = view->selectionModel();
        QModelIndex newItemIndex = model->index(model->rowCount(QModelIndex()) - 1);
        
        selectionModel->clearSelection();
        selectionModel->select(newItemIndex, QItemSelectionModel::Select);
        
        view->setFocus();
    }
}

void 
EditorController::removeTask()
{
    QModelIndexList indexes = m_editWorkTaskPage->selectedTasks();
    QListView* view = m_editWorkTaskPage->workTasksView();
    
    SelectedWorkDayModel* model = qobject_cast<SelectedWorkDayModel*>(view->model());
    
    if (0 == indexes.size()) {
        return;
    }
    
    QString question;
    if (1 == indexes.size()) {
        WorkTask workTask = model->workTask(indexes.at(0));
        
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
    
    int result = QMessageBox::question(m_editWorkTaskPage, tr("Delete Task"), question, 
                                       QMessageBox::Yes, QMessageBox::No);
    
    if (QMessageBox::Yes == result) {
        model->removeTasks(indexes);
    }
}

void 
EditorController::addTime()
{
    QTableView* view = m_editWorkTaskPage->workTimesView();
    WorkTaskModel* model = qobject_cast<WorkTaskModel*>(view->model());
    model->appendTime();
    
    QItemSelectionModel* selectionModel = view->selectionModel();
    QModelIndex newItemIndex = model->index(model->rowCount(QModelIndex()) - 1, 0);
    
    selectionModel->clearSelection();
    selectionModel->select(newItemIndex, QItemSelectionModel::Select);
    
    view->setFocus();
}

void 
EditorController::removeTime()
{
    QModelIndexList indexes = m_editWorkTaskPage->selectedTimes();
    if (!indexes.isEmpty()) {
        int result = QMessageBox::question(
                    m_editWorkTaskPage, tr("Delete Time"), 
                    tr("Are you sure you want to delete %1 recorded times?")
                      .arg(indexes.size()), 
                    QMessageBox::Yes, QMessageBox::No);
        
        if (QMessageBox::Yes == result) {
            QTableView* view = m_editWorkTaskPage->workTimesView();
            WorkTaskModel* model = qobject_cast<WorkTaskModel*>(view->model());
            model->removeTimes(indexes);
        }
    }
}

QDomDocument 
EditorController::dataSource() const
{
    return m_dataSource;
}

void 
EditorController::validateModel()
{
    QVariant value = m_selectWorkDayPage->selectedItem();
    if (value.canConvert<WorkDay>()) {
        WorkDay workDay = qvariant_cast<WorkDay>(value);
        
        QStringList activeTasks;
        
        QList<WorkTask> workTasks = workDay.workTasks();
        for (const WorkTask& workTask : workTasks) {
            QString taskName = workTask.task().name();
            
            if (taskName.isEmpty()) {
                emit validationError(tr("There is a work-task without a name"));
                return;
            }
            
            QList<WorkTime> workTimes = workTask.workTimes();
            for (const WorkTime& workTime : workTimes) {
                if (workTime.start().isNull()) {
                    emit validationError(tr("Work-task \"%1\" has no start time")
                                         .arg(workTask.task().name()));
                    return;
                }
                
                if (workTime.stop().isNull()) {
                    if (!activeTasks.contains(taskName)) {
                        activeTasks << taskName;
                    }
                }
            }
        }
        
        if (1 < activeTasks.size()) {
            emit validationError(tr("The following tasks are active: %1")
                                 .arg(activeTasks.join(",")));
            return;
        }
    }
    
    emit validationSuccess();
}

void 
EditorController::updateActiveWorkTasks()
{
    // Find the latest workday and see what's active and what is not. The latest work-day
    // is number of work-days minus 1 (zero-based index).
    int countDays = WorkDay::count(m_dataSource);
    WorkDay workDay = WorkDay::at(countDays - 1, m_dataSource);
    
    if (!workDay.isNull()) {
        WorkTask activeWorkTask;
        
        QList<WorkTask> workTasks = workDay.workTasks();
        for (const WorkTask& workTask : workTasks) {
            if (workTask.task().isNull()) {
                continue;
            }
            
            if (workTask.isActiveTask()) {
                activeWorkTask = workTask;
                break;
            }
        }
        
        if (activeWorkTask.isNull()) {
            emit closeCurrentTask();
        }
        else {
            emit setActiveTask(activeWorkTask);
        }
    }
}
