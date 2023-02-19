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

#include "worktrackercontroller.h"
#include "preferencescontroller.h"
#include "../helper.h"

#include <QDateTime>
#include <QDomDocument>
#include <QDebug>
//#include <QMapIterator>

#include <algorithm>

static const int TIMER_TIMEOUT = 30 * 1000;

WorkTrackerController::WorkTrackerController(QString databaseLocation)
    : QObject{}
    , AbstractController{std::move(databaseLocation)}
    , m_isNewWorkDay(false)
    , m_isRecording(false)
{
    m_timer.setInterval(TIMER_TIMEOUT);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(this,     SIGNAL(workTaskStarted(QDateTime, QString)), 
            &m_timer, SLOT(start()));
    connect(this,     SIGNAL(workTaskStopped(QDateTime, QString)), 
            &m_timer, SLOT(stop()));
    connect(this,     SIGNAL(workDayStopped(QDateTime)), 
            &m_timer, SLOT(stop()));
}

bool
WorkTrackerController::load()
{
    // Do nothing if the datasource is already loaded.
    if (!m_dataSource.isNull()) {
        return true;
    }

    if (!m_dataSource.load()) {
        qDebug() << "Could not load database.";
        return false;
    }
    else {
        m_taskList.setDataSource(m_dataSource);
        m_workDayList.setDataSource(m_dataSource);

        m_workday      = m_workDayList.findLastOpen();
        m_isNewWorkDay = !m_workday.isNull();
        if (m_isNewWorkDay) {
            emit workDayStarted(m_workday.start());

            m_recordingWorkTask = m_workday.activeWorkTask();
            m_recordingWorkTime = m_recordingWorkTask.activeTime();
            m_isRecording = !m_recordingWorkTime.isNull();
            if (m_isRecording) {
                m_timer.start(TIMER_TIMEOUT);
                emit workTaskStarted(m_recordingWorkTime.start(),
                                     m_recordingWorkTask.task().name());
            }
        }

        return true;
    }
}

void
WorkTrackerController::toggleWorkDay()
{
    auto now = QDateTime::currentDateTimeUtc();
    if (!m_isNewWorkDay) {
        startWorkDay(now);
    }
    else {
        stopWorkDay(now);
    }

    m_isNewWorkDay = !m_isNewWorkDay;
}

void
WorkTrackerController::toggleTask(const QString& name)
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
    if (!m_dataSource.save()) {
        qDebug() << "Could not save database.";
    }
}

bool
WorkTrackerController::isRecording() const
{
    return m_isRecording;
}

bool 
WorkTrackerController::isActiveDay() const
{
    return m_isNewWorkDay;
}

QString
WorkTrackerController::generateSummary() const
{
    if (m_workday.isNull()) {
        // Pick the last day from the database until the user starts a new one. This situation
        // occurs when the application starts but finds no active workday.
        if (0 == m_workDayList.size()) {
            return tr("No previous data available");
        }

        auto last = m_workDayList.at(m_workDayList.size() - 1);
        if (last.isNull()) {
            return tr("No previous data available");
        }
        return last.generateSummary();
    }
    else {
        return m_workday.generateSummary();
    }
}

void
WorkTrackerController::startWorkDay(const QDateTime& p_now)
{
    m_workday = WorkDay{m_dataSource, p_now, QDateTime{}};

    auto root = m_dataSource.document().documentElement();
    auto days = root.firstChildElement("workdays");

    auto elem = m_workday.element();
    if (elem.isNull()) {
        emit error(tr("Could not create <workday> XML element"));
    }

    auto result = days.appendChild(elem);
    if (result.isNull()) {
        emit error(tr("Could not add <workday> to <workdays> XML element"));
    }

    emit workDayStarted(p_now);
}

void
WorkTrackerController::stopWorkDay(const QDateTime& now)
{
    if (m_workday.isNull()) {
        emit error(tr("No active work-day"));
    }

    m_workday.setStop(now);

    emit workDayStopped(now);
}

void
WorkTrackerController::startWorkTask(const QString& name)
{
    if (name.isEmpty()) {
        emit error(tr("A task-name must be specified to start a new task"));
        return;
    }
    
    auto now  = QDateTime::currentDateTimeUtc();
    auto task = findOrCreateTask(name);

    // Always reset before starting a new one.
    m_recordingWorkTime = WorkTime{m_dataSource, now, QDateTime{}};
    m_recordingWorkTask = m_workday.findWorkTask(task);
    if (m_recordingWorkTask.isNull()) {
        m_recordingWorkTask = WorkTask(m_dataSource, task);
    }

    m_recordingWorkTask.addTime(m_recordingWorkTime);

    // If the work-task has no parent it means it wasn't obtained from the workday and
    // thus needs to be added right now, to have in the database in case the application 
    // is closed. 
    if (m_recordingWorkTask.parent().isNull()) {
        m_workday.addWorkTask(m_recordingWorkTask);
    }

    emit workTaskStarted(now, name);
}

void
WorkTrackerController::stopWorkTask(const QString& name)
{
    // If the currently recording task and the new task are the same then we can continue
    // and add the stop timestamp to our current values and be done with it. If the tasks
    // are different then we have to re-assign the work-time from the current work-task
    // to the new one.
    auto newTask = findOrCreateTask(name);
    auto recTask = m_recordingWorkTask.task();
    
    if (newTask.id() != recTask.id()) {
        auto otherWorkTask = m_workday.findWorkTask(newTask);
        if (otherWorkTask.isNull()) {
            otherWorkTask = WorkTask{m_dataSource, newTask};
            m_workday.addWorkTask(otherWorkTask);
        }
        
        otherWorkTask.addTime(m_recordingWorkTime);
    }
    
    auto timestamp = QDateTime::currentDateTimeUtc();
    if (m_recordingWorkTime.stop().isNull()) {
        m_recordingWorkTime.setStop(timestamp);
    }
    else {
        timestamp = m_recordingWorkTime.stop();
    }
    
    // Always reset after it is not needed any more
    m_recordingWorkTask = WorkTask{};
    m_recordingWorkTime = WorkTime{};

    emit workTaskStopped(timestamp, name);
}

void
WorkTrackerController::timeout()
{
    auto totalMinutes = static_cast<int>(m_workday.totalTime() / 60);

    auto hours   = static_cast<int>(totalMinutes / 60);
    auto minutes = static_cast<int>(totalMinutes % 60);

    emit totalTimeChanged(hours, minutes);
}

void 
WorkTrackerController::setActiveTask(const WorkTask& p_task)
{
    if (p_task.isNull() || p_task.task().name().isEmpty()) {
        return; // Sorry pal, can't help ya.
    }
    
    if (m_recordingWorkTask != p_task) {
        closeCurrentTask();
        m_recordingWorkTask = p_task;
    }
    
    auto name = m_recordingWorkTask.task().name();
    auto now  = QDateTime::currentDateTimeUtc();
    
    m_recordingWorkTime = p_task.activeTime();
    if (!m_recordingWorkTime.isNull()) {
        if (m_recordingWorkTime.start().isNull()) {
            m_recordingWorkTime.setStart(now);
        }
        
        m_isRecording = true;
        emit workTaskStarted(m_recordingWorkTime.start(), name);
    }
    else {
        m_isRecording = false;
        emit workTaskStopped(now, name);
    }
}

void 
WorkTrackerController::closeCurrentTask()
{
    if (m_recordingWorkTask.isNull()) {
        return; // Nothing to do here
    }
    
    if (m_isRecording) {
        // Simply rely on what we already have. This also takes care about sending the 
        // necessary signal to update the ui.
        stopWorkTask(m_recordingWorkTask.task().name());
        m_isRecording = false;
    }
}

Task 
WorkTrackerController::findOrCreateTask(const QString& name)
{
    auto now   = QDateTime::currentDateTimeUtc();
    auto found = m_taskList.find(name);
    if (found.isNull()) {
        found = Task(m_dataSource, name, now);
        m_taskList.add(found);
    }
    else {
        found.setLastUsed(now);
    }
    return found;
}
