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
