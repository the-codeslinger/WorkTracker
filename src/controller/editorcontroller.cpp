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
#include "../selectworkdaypage.h"
#include "../editworktaskpage.h"
#include "../model/ui/workdaymodel.h"
#include "../model/ui/worktaskmodel.h"
#include "../model/ui/selectedworkdaymodel.h"
#include "../selecttaskdialog.h"

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
    QWizard wizard(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                            | Qt::WindowCloseButtonHint);
    wizard.setWindowTitle(tr("Edit Work Tasks"));

    m_selectWorkDayPage = new SelectWorkDayPage(this, &wizard);
    m_editWorkTaskPage  = new EditWorkTaskPage(this, &wizard);

    wizard.addPage(m_selectWorkDayPage);
    wizard.addPage(m_editWorkTaskPage);
    //wizard.setModal(true);
    QSize size = wizard.size();
    wizard.resize(size.width() * 1.1, size.height() * 1.1);
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
        
        // Don't care about the return value. The emitted signal concerning duplicates
        // is handled in the page. That's the only realistic scenario of failure.
    }
}

void 
EditorController::removeTask()
{
    QModelIndex index = m_editWorkTaskPage->selectedTask();
    QListView*  view  = m_editWorkTaskPage->workTasksView();
    
    SelectedWorkDayModel* model = qobject_cast<SelectedWorkDayModel*>(view->model());
    WorkTask workTask = model->workTask(index);
    
    if (workTask.isNull()) {
        return;
    }
    
    int result = QMessageBox::question(
                m_editWorkTaskPage, tr("Delete Task"), 
                tr("Are you sure you want to delete the work-task \"%1\" \n"
                   "and all of its recorded times?").arg(workTask.task().name()), 
                QMessageBox::Yes, QMessageBox::No);
    
    if (QMessageBox::Yes == result) {
        model->removeTask(index);
    }
}

void 
EditorController::addTime()
{
    QTableView* view = m_editWorkTaskPage->workTimesView();
    WorkTaskModel* model = qobject_cast<WorkTaskModel*>(view->model());
    model->appendTime();
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
