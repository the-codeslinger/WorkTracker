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

#include "preferencescontroller.h"
#include "../ui/preferencesdialog.h"

#include <QSettings>

PreferencesController::PreferencesController(QWidget* p_parentWidget, QObject* p_parent)
    : QObject{p_parent}
    , m_dialog{new PreferencesDialog{this, p_parentWidget}}
{
    connect(m_dialog, &PreferencesDialog::accepted, 
            this,     &PreferencesController::saveSettings);
    connect(m_dialog, &PreferencesDialog::rejected, 
            this,     &PreferencesController::restoreSettings);
}

void
PreferencesController::run()
{
    m_dialog->show();
}

void 
PreferencesController::saveSettings()
{
    emit finished(true);
}

void 
PreferencesController::restoreSettings()
{
    emit finished(false);
}
