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
#include "../model/workdaylist.h"
#include "../model/workday.h"

#include <QStringList>
#include <QDateTime>

#include <algorithm>

EditorController::EditorController(DataSource dataSource, QObject* parent)
    : QObject{parent}
    , AbstractController{std::move(dataSource)}
{ }

void 
EditorController::validateModel(const WorkDay& workDay)
{
    auto activeTasks = QStringList{};
        
    auto workTasks = workDay.workTasks();
    for (const auto& workTask : workTasks) {
        auto taskName = workTask.task().name();
            
        if (taskName.isEmpty()) {
            emit validationError(tr("There is a work-task without a name"));
            return;
        }
            
        auto workTimes = workTask.times();
        for (const auto& workTime : workTimes) {
            if (workTime.start().isNull()) {
                emit validationError(tr("Work-task \"%1\" has no start time")
                                       .arg(workTask.task().name()));
                return;
            }
                
            if (workTime.stop().isNull()) {
                activeTasks << taskName;
            }
        }
    }
        
    if (1 < activeTasks.size()) {
        activeTasks.removeDuplicates();
        emit validationError(
                tr("The following tasks are active or have more than one active time: %1")
                  .arg(activeTasks.join(",")));
        return;
    }
    
    emit validationSuccess();
}

void 
EditorController::updateActiveWorkTasks()
{
    // Find the latest workday and see what's active and what is not. The latest work-day
    // is number of work-days minus 1 (zero-based index).
    auto workDayList = WorkDayList{m_dataSource};
    auto countDays   = workDayList.size();
    auto workDay     = workDayList.at(countDays - 1);
    
    if (!workDay.isNull()) {
        auto activeWorkTask = WorkTask{};
        
        auto workTasks = workDay.workTasks();
        for (const auto& workTask : workTasks) {
            if (workTask.task().isNull()) {
                continue;
            }
            
            if (workTask.isActiveTask()) {
                activeWorkTask = workTask;
                break;
            }
        }
        
        if (activeWorkTask.isNull()) {
            emit currentTaskClosed();
        }
        else {
            emit activeTaskChanged(activeWorkTask);
        }
    }
}
