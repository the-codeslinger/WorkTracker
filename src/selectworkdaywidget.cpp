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

#include "selectworkdaywidget.h"
#include "ui_selectworkdaywidget.h"
#include "controller/editorcontroller.h"
#include "model/ui/workdaymodel.h"
#include "model/delegate/workdaydelegate.h"

SelectWorkdayWidget::SelectWorkdayWidget(EditorController* p_controller, QWidget* p_parent)
    : QWidget(p_parent)
    , ui(new Ui::SelectWorkdayWidget)
{
    ui->setupUi(this);
    ui->daysListView->setModel(p_controller->workDayModel(ui->daysListView));
    ui->daysListView->setItemDelegate(new WorkDayDelegate(ui->daysListView));

}

SelectWorkdayWidget::~SelectWorkdayWidget()
{
    delete ui;
}
