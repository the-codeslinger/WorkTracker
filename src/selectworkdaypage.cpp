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

#include "selectworkdaypage.h"
#include "selectworkdaywidget.h"
#include "controller/editorcontroller.h"

#include <QVBoxLayout>
#include <QItemSelectionModel>

SelectWorkdayPage::SelectWorkdayPage(EditorController* p_controller, QWidget* p_parent)
    : QWizardPage(p_parent)
    , m_isComplete(false)
{
    SelectWorkdayWidget* widget = new SelectWorkdayWidget(p_controller, this);
    QItemSelectionModel* select = widget->selectionModel();
    setTitle(tr("Select Workday"));
    setSubTitle(tr("Select the specific workday for which you want to edit individual "
                   "task items. You can always come back to this page."));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(widget);

    connect(select, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,   SLOT(itemSelected(QItemSelection)));

    setLayout(layout);
}

bool
SelectWorkdayPage::isComplete() const
{
    return m_isComplete;
}

void
SelectWorkdayPage::itemSelected(const QItemSelection& p_selected)
{
    m_isComplete = !p_selected.isEmpty();
    emit completeChanged();
}
