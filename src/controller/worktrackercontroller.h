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

#ifndef WORKTRACKERCONTROLLER_H
#define WORKTRACKERCONTROLLER_H

#include "abstractcontroller.h"
#include "../model/workday.h"
#include "../model/worktask.h"
#include "../model/worktime.h"
#include "../model/tasklist.h"
#include "../model/workdaylist.h"

#include <QObject>
#include <QTimer>
#include <QString>

class QDateTime;

/*!
 * \brief Contains the business logic of the WorkTracker application.
 *
 * The `WorkTrackerController` brings the UI and the database together and coordinates
 * user actions in the UI with the data stored in the XML file. The controller doesn't
 * have many methods to interact with. There are a few to set it up (`run()`,
 * `setUi(WorkTracker*)`) and exactly two that perform the task of tracking tasks.
 * `toggleWorkDay()` either starts a new or stops a running workday, based on the current
 * state of the application. The same goes for `toggleTask(const QString&)`. Both methods
 * emit signals to convey the current status as a result of the `toggle*()` methods.
 */
class WorkTrackerController : public QObject, public AbstractController
{
    Q_OBJECT
public:
    /*!
     * Create a new controller.
     * 
     * \param[in] location
     * Full filename of the database if specified by the user on the command line.
     */
    WorkTrackerController(QString databaseLocation);

    /*!
     * Search the database for the last open workday and load it. Emits 
     * `WorkTrackerController::workDayStarted(QDateTime)` if an ongoing day is found and
     * `WorkTrackerController::workTaskStarted(QDateTime, QString)` for an active task.
     * 
     * \return
     * Returns `true` on success or `false` if there was a problem initializing the
     * database.
     */
    bool load();

    /*!
     * Closes any open task in order to not loose any data.
     */
    void close();

    /*!
     * \return
     * Returns `true` if a task is currently recorded or `false` otherwise.
     */
    bool isRecording() const;
    
    /*!
     * \return
     * Returns `true` if a day is currently active or `false` otherwise.
     */
    bool isActiveDay() const;

    /*!
     * \return
     * Returns a summary based on the recorded worktasks in the current workday.
     *
     * \see
     * WorkDay::generateSummary() const
     */
    QString generateSummary() const;

signals:
    /*!
     * Emitted as a result to a call to `toggleWorkDay()` when a new workday has been
     * started.
     *
     * \param start
     * The timestamp of the start in UTC format.
     */
    void workDayStarted(const QDateTime& start);

    /*!
     * Emitted as a result to a call to `toggleWorkDay()` when a workday has been
     * finished.
     *
     * \param now
     * The timestamp of the stop in UTC format.
     */
    void workDayStopped(const QDateTime& now);

    /*!
     * Emitted as a result to a call to `toggleTask(QString)` when a worktask has been
     * started.
     *
     * \param now
     * The timestamp of the start in UTC format.
     *
     * \param name
     * The name of the task.
     */
    void workTaskStarted(const QDateTime& now, const QString& name);

    /*!
     * Emitted as a result to a call to `toggleTask(const QString&)` when a worktask has
     * been finished.
     *
     * \param now
     * The timestamp of the stop in UTC format.
     *
     * \param name
     * The name of the task.
     */
    void workTaskStopped(const QDateTime& now, const QString& name);

    /*!
     * Emitted whenever there is an error processing the `toggle*()` methods.
     *
     * \param text
     * Error description.
     */
    void error(const QString& text);

    /*!
     * Emitted every minute if the status is recording, which means that a task is
     * currently running.
     *
     * \param hours
     * The amount of hours already spent on tasks.
     *
     * \param minutes
     * The amount of minutes of a started hour already spent on tasks.
     */
    void totalTimeChanged(int hours, int minutes);

public slots:
    /*!
     * Starts or stops a new workday. The current state is managed by the controller and
     * the signal `workDayStarted(const QDateTime&)` is emitted if a new day has been started or
     * `workDayStopped(const QDateTime&)` if a running day has been finished.
     */
    void toggleWorkDay();

    /*!
     * Starts or stops a new worktask. The current state is managed by the controller and
     * the signal `workTaskStarted(const QDateTime&, const QString&)` is emitted if a new task has been
     * started or `workTaskStopped(const QDateTime&, const QString&)` if a running task has been
     * finished.
     */
    void toggleTask(const QString& name);
    
    /*!
     * Replaces the currently running task with a new one. If the current task is not yet
     * finished then the current time is used.
     * 
     * \param p_task
     * The new task to replace an existing one. It is only accepted if `WorkTask::task()`
     * returns a valid instance. If the start time is missing the current one will be 
     * used.
     */
    void setActiveTask(const WorkTask& p_task);
    
    /*!
     * Stops recording time for the current task. This method can be used to "flush the
     * pipeline" if the task has been modified outside the controller (e.g. through the
     * editor). If the task doesn't have a stop time the current one will be used.
     */
    void closeCurrentTask();

    /*!
     * Connected to q `QTimer` once a task is running. Calculates the total sum of all
     * tasks and displays it through the UI.
     */
    void timeout();

private:
    /*!
     * The currently recorded workday.
     */
    WorkDay m_workday;
    /*!
     * The currently recorded task.
     */
    WorkTask m_recordingWorkTask;
    /*!
     * The current start and stop time of the work-task.
     */
    WorkTime m_recordingWorkTime;
    /*!
     * The list of all known tasks in the database.
     */
    TaskList m_taskList;
    /*!
     * The list of all known workdays in the database.
     */
    WorkDayList m_workDayList;

    /*!
     * The current state of the workday.
     */
    bool m_isNewWorkDay;
    /*!
     * The current state of the task.
     */
    bool m_isRecording;
    /*!
     * The timer is used to fire every minute when a task is currently running.
     */
    QTimer m_timer;

    /*!
     * The actual implementation that starts a new workday. Called by `toggleWorkDay()`.
     */
    void startWorkDay(const QDateTime& now);

    /*!
     * The actual implementation that stops a running workday. Called by
     * `toggleWorkDay()`.
     */
    void stopWorkDay(const QDateTime& now);

    /*!
     * The actual implementation that starts a new task. Called by `toggleTask(const QString&)`.
     */
    void startWorkTask(const QString& name);

    /*!
     * The actual implementation that stops a running task. Called by
     * `toggleTask(const QString&)`.
     */
    void stopWorkTask(const QString& name);

    /*!
     * Looks up the task name in the list of tasks and returns the found one or creates 
     * and adds a new task.
     */
    Task findOrCreateTask(const QString& name);
};

#endif // WORKTRACKERCONTROLLER_H
