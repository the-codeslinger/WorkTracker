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

#include "selectworkdaypage.h"
#include "ui_selectworkdaywidget.h"
#include "../controller/editorcontroller.h"
#include "model/workdaymodel.h"
#include "delegate/workdaydelegate.h"

#include <QVBoxLayout>
#include <QItemSelectionModel>

SelectWorkDayPage::SelectWorkDayPage(EditorController* p_controller, QWidget* p_parent)
    : QWizardPage{p_parent}
    , ui{new Ui::SelectWorkdayWidget}
    , m_controller{p_controller}
    , m_isComplete{false}
{
    ui->setupUi(this);
    ui->daysListView->setModel(new WorkDayModel{ui->daysListView});
    ui->daysListView->setItemDelegate(new WorkDayDelegate{ui->daysListView});
    ui->daysListView->setMouseTracking(true);

    setTitle(QObject::tr("Select Workday"));
    setSubTitle(QObject::tr("Select the specific workday for which you want to edit "
                            "individual task items. You can always come back to this "
                            "page."));

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
        auto index = ui->daysListView->model()->index(0, 0);
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
    auto* model = ui->daysListView->model();
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
