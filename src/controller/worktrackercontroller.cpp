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

#include "worktrackercontroller.h"
#include "../model/ui/tasklistmodel.h"
#include "../worktracker.h"

#include <QDomDocument>
#include <QDateTime>
#include <QDebug>

WorkTrackerController::WorkTrackerController(QDomDocument* dataSource)
    : m_dataSource(dataSource)
    , m_taskListModel(nullptr)
    , m_workday(dataSource)
    , m_recordingWorkTask(dataSource)
    , m_isNewWorkDay(false)
    , m_isRecording(false)
{
}

void
WorkTrackerController::setUi(WorkTracker* ui)
{
    m_ui = ui;
    m_taskListModel = new TaskListModel(m_dataSource, this);
    m_ui->setTaskListModel(m_taskListModel);
}

void
WorkTrackerController::run()
{
    m_workday = WorkDay::findLastOpen(m_dataSource);

    // The UI needs to be shown first or otherwise the displaying the status text for a
    // running task can end up very short. If there's no UI then the shortening algorithm
    // won't know the true available length of the label.
    m_ui->show();

    m_isNewWorkDay = !m_workday.isNull();
    if (m_isNewWorkDay) {
        emit workDayStarted(m_workday.start());

        m_recordingWorkTask = m_workday.runningWorkTask();
        m_isRecording = !m_recordingWorkTask.isNull();
        if (m_isRecording) {
            emit workTaskStarted(m_recordingWorkTask.start(),
                                 m_recordingWorkTask.task().name());
        }
    }
}

void
WorkTrackerController::toggleWorkDay()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    if (!m_isNewWorkDay) {
        startWorkDay(now);
    }
    else {
        stopWorkDay(now);
    }

    m_isNewWorkDay = !m_isNewWorkDay;
}

void
WorkTrackerController::toggleTask(QString name)
{
    if (m_isRecording) {
        stopWorkTask(name);
    }
    else {
        startWorkTask(name);
    }

    m_isRecording = !m_isRecording;
}

void
WorkTrackerController::close()
{
    // Nothing to do (any more) at the moment
}

bool
WorkTrackerController::isRecording() const
{
    return m_isRecording;
}

QString
WorkTrackerController::generateSummary() const
{
    return m_workday.generateSummary();
}

void
WorkTrackerController::startWorkDay(QDateTime now)
{
    m_workday = WorkDay(m_dataSource, now);

    QDomElement root = m_dataSource->documentElement();
    QDomElement days = root.firstChildElement("workdays");

    QDomNode elem = m_workday.node();
    if (elem.isNull()) {
        emit error(tr("Could not create <workday> XML element"));
    }

    QDomNode result = days.appendChild(elem);
    if (result.isNull()) {
        emit error(tr("Could not add <workday> to <workdays> XML element"));
    }

    emit workDayStarted(now);
}

void
WorkTrackerController::stopWorkDay(QDateTime now)
{
    if (m_workday.isNull()) {
        emit error(tr("No active work-day"));
    }

    m_workday.setStop(now);

    emit workDayStopped(now);
}

void
WorkTrackerController::startWorkTask(QString name)
{
    // Always reset before starting a new one
    m_recordingWorkTask = WorkTask(m_dataSource);

    QDateTime now = QDateTime::currentDateTimeUtc();
    m_recordingWorkTask.setStart(now);
    if (!name.isEmpty()) {
        Task task = findOrCreateTaskItem(name);
        m_recordingWorkTask.setTask(task);

        // Add the task right now, to have in the database in case the application is
        // closed
        m_recordingWorkTask = m_workday.addTask(m_recordingWorkTask);
    }

    emit workTaskStarted(now, name);
}

void
WorkTrackerController::stopWorkTask(QString name)
{
    // First check if there's already a task attached and if it is a different one. Maybe
    // we don't need to look for one
    Task taskItem = m_recordingWorkTask.task();
    if (!taskItem.isNull()) {
        if (taskItem.name() != name) {
            taskItem = findOrCreateTaskItem(name);

            // Set the new task item
            m_recordingWorkTask.setTask(taskItem);
        }
    }

    QDateTime now = QDateTime::currentDateTimeUtc();

    m_recordingWorkTask.setStop(now);
    // Add the worktask again, to have it written with the stop date. This also moves the
    // node to another task if it changed.
    m_recordingWorkTask = m_workday.addTask(m_recordingWorkTask);
    // Always reset after it is not needed any more
    m_recordingWorkTask = WorkTask(m_dataSource);

    emit workTaskStopped(now, name);
}

Task
WorkTrackerController::findOrCreateTaskItem(QString name)
{
    Task taskItem = Task::findByName(name, m_dataSource);
    if (taskItem.isNull()) {
        // Create a new one
        taskItem = Task(m_dataSource, Task::count(m_dataSource), name,
                        QDate::currentDate());
        if (taskItem.node().isNull()) {
            emit error(tr("Could not create <item> XML element"));
            return Task();
        }

        QDomElement root   = m_dataSource->documentElement();
        QDomElement tasks  = root.firstChildElement("tasks");
        QDomNode    result = tasks.appendChild(taskItem.node());
        if (result.isNull()) {
            emit error(tr("Could not add <item> to <tasks> XML element"));
            return Task();
        }

        m_taskListModel->itemAppended();
    }

    return taskItem;
}
