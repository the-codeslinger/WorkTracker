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
#include "preferencescontroller.h"
#include "../helper.h"

#include <QDomDocument>
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include <QMapIterator>
#include <QSettings>

static const int TIMER_TIMEOUT = 30 * 1000;

WorkTrackerController::WorkTrackerController(const QString& databaseLocation)
    : QObject()
    , AbstractController(databaseLocation)
    , m_isNewWorkDay(false)
    , m_isRecording(false)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
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
    return m_workday.generateSummary();
}

void
WorkTrackerController::startWorkDay(QDateTime p_now)
{
    m_workday = WorkDay(m_dataSource, p_now, QDateTime());

    QDomElement root = m_dataSource.document().documentElement();
    QDomElement days = root.firstChildElement("workdays");

    QDomNode elem = m_workday.element();
    if (elem.isNull()) {
        emit error(tr("Could not create <workday> XML element"));
    }

    QDomNode result = days.appendChild(elem);
    if (result.isNull()) {
        emit error(tr("Could not add <workday> to <workdays> XML element"));
    }

    emit workDayStarted(p_now);
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
    if (name.isEmpty()) {
        emit error(tr("A task-name must be specified to start a new task"));
        return;
    }
    
    m_timer.start(TIMER_TIMEOUT);
    
    QDateTime now = QDateTime::currentDateTimeUtc();
    Task task = findOrCreateTask(name);

    // Always reset before starting a new one.
    m_recordingWorkTime = WorkTime(m_dataSource, now, QDateTime());
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
WorkTrackerController::stopWorkTask(QString name)
{
    m_timer.stop();
    
    // If the currently recording task and the new task are the same then we can continue
    // and add the stop timestamp to our current values and be done with it. If the tasks
    // are different then we have to re-assign the work-time from the current work-task
    // to the new one.
    Task newTask = findOrCreateTask(name);
    
    Task recTask = m_recordingWorkTask.task();
    
    if (newTask.id() != recTask.id()) {
        WorkTask otherWorkTask = m_workday.findWorkTask(newTask);
        if (otherWorkTask.isNull()) {
            otherWorkTask = WorkTask(m_dataSource, newTask);
            m_workday.addWorkTask(otherWorkTask);
        }
        
        otherWorkTask.addTime(m_recordingWorkTime);
    }
    
    QDateTime timestamp = QDateTime::currentDateTimeUtc();
    if (m_recordingWorkTime.stop().isNull()) {
        m_recordingWorkTime.setStop(timestamp);
    }
    else {
        timestamp = m_recordingWorkTime.stop();
    }
    
    // Always reset after it is not needed any more
    m_recordingWorkTask = WorkTask();
    m_recordingWorkTime = WorkTime();

    emit workTaskStopped(timestamp, name);
}

void
WorkTrackerController::timeout()
{
    int totalMinutes = static_cast<int>(m_workday.totalTime() / 60);

    int hours   = static_cast<int>(totalMinutes / 60);
    int minutes = static_cast<int>(totalMinutes % 60);

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
    
    QString name  = m_recordingWorkTask.task().name();
    QDateTime now = QDateTime::currentDateTimeUtc();
    
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
    Task found = m_taskList.find(name);
    if (found.isNull()) {
        found = Task(m_dataSource, name, QDate::currentDate());
        m_taskList.add(found);
    }
    return found;
}
