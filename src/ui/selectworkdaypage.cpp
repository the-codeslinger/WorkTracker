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
#include "ui_selectworkdaywidget.h"
#include "../controller/editorcontroller.h"
#include "model/workdaymodel.h"
#include "delegate/workdaydelegate.h"

#include <QVBoxLayout>
#include <QItemSelectionModel>

SelectWorkDayPage::SelectWorkDayPage(EditorController* p_controller, QWidget* p_parent)
    : QWizardPage(p_parent)
    , ui(new Ui::SelectWorkdayWidget)
    , m_controller(p_controller)
    , m_isComplete(false)
{
    ui->setupUi(this);
    ui->daysListView->setModel(new WorkDayModel(ui->daysListView));
    ui->daysListView->setItemDelegate(new WorkDayDelegate(ui->daysListView));

    setTitle(tr("Select Workday"));
    setSubTitle(tr("Select the specific workday for which you want to edit individual "
                   "task items. You can always come back to this page."));

    connect(ui->daysListView, SIGNAL(clicked(QModelIndex)),
            this,             SLOT(itemSelected(QModelIndex)));
}

SelectWorkDayPage::~SelectWorkDayPage()
{
    delete ui;
}

bool
SelectWorkDayPage::isComplete() const
{
    return m_isComplete;
}

void
SelectWorkDayPage::initializePage()
{
    auto* model = qobject_cast<WorkDayModel*>(ui->daysListView->model());
    model->setDataSource(m_controller->dataSource());

    if (0 < ui->daysListView->model()->rowCount()) {
        QModelIndex index = ui->daysListView->model()->index(0, 0);
        ui->daysListView->selectionModel()->select(index, QItemSelectionModel::Select);
        itemSelected(index);
    }
}

void
SelectWorkDayPage::itemSelected(const QModelIndex& p_index)
{
    m_isComplete = p_index.isValid();
    emit completeChanged();
}

QVariant
SelectWorkDayPage::selectedItem() const
{
    // The index returned could be invalid, but the model can cope with that. In that
    // case we'll get an invalid QVariant back
    QAbstractItemModel* model = ui->daysListView->model();
    return model->data(ui->daysListView->selectionModel()->currentIndex());
}

void 
SelectWorkDayPage::changeEvent(QEvent* p_event)
{
    if (QEvent::LanguageChange == p_event->type()) {
        ui->retranslateUi(this);
    }
    
    QWizardPage::changeEvent(p_event);
}
