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
	if (0 < countDays) {
		auto workDay = workDayList.at(countDays - 1);
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
}
