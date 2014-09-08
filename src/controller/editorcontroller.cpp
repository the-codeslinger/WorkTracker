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

#include <QWizard>

EditorController::EditorController(QDomDocument* p_dataSource, QObject* p_parent)
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
    p_destination->setWorkTasks(p_source->workTasks(p_index));
}

void 
EditorController::addTask()
{
    
}

void 
EditorController::removeTask()
{
    
}

void 
EditorController::addTime()
{
    
}

void 
EditorController::removeTime()
{
    
}
