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

#include "editorwizard.h"
#include "selectworkdaypage.h"
#include "editworktaskpage.h"
#include "../controller/editorcontroller.h"

#include <QPushButton>

EditorWizard::EditorWizard(EditorController* controller, QWidget* parent)
    : QWizard{parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                      | Qt::WindowCloseButtonHint}
    , m_controller{controller}
    , m_resized{false}
{
    setWindowTitle(tr("Edit Work Tasks"));

    m_selectWorkDayPage = new SelectWorkDayPage{controller, this};
    m_editWorkTaskPage  = new EditWorkTaskPage{controller, m_selectWorkDayPage, this};

    addPage(m_selectWorkDayPage);
    addPage(m_editWorkTaskPage);
    
    connect(controller, &EditorController::validationSuccess,
            this,       &EditorWizard::validationSuccess);
    connect(controller, &EditorController::validationError,
            this,       &EditorWizard::validationError);
    connect(this,       &EditorWizard::finished,
            controller, &EditorController::updateActiveWorkTasks);
}

void
EditorWizard::validationError()
{
    button(QWizard::BackButton)->setDisabled(true);
    button(QWizard::FinishButton)->setDisabled(true);
}

void 
EditorWizard::validationSuccess()
{
    button(QWizard::BackButton)->setDisabled(false);
    button(QWizard::FinishButton)->setDisabled(false);
}

void 
EditorWizard::showEvent(QShowEvent* event)
{
    QWizard::showEvent(event);
    if (!m_resized) {
        m_resized = true;
        
        auto s = size();
        resize(s.width() * 1.10, s.height() * 1.10);
    }
}
