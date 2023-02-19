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
    setWindowTitle(QObject::tr("Edit Work Tasks"));

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
        resize(s.width() * 1.30, s.height() * 1.30);
    }
}
