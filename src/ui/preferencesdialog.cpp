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

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "../controller/preferencescontroller.h"

#include <QMapIterator>
#include <QApplication>
#include <QComboBox>

PreferencesDialog::PreferencesDialog(PreferencesController* p_controller, QWidget* p_parent)
    : QDialog{p_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
                        | Qt::WindowCloseButtonHint}
    , ui{new Ui::PreferencesDialog}
    , m_controller{p_controller}
{
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void
PreferencesDialog::changeEvent(QEvent* p_event)
{
    if (QEvent::LanguageChange == p_event->type()) {
        ui->retranslateUi(this);
    }

    QDialog::changeEvent(p_event);
}
