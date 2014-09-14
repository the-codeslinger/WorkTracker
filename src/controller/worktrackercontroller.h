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

#ifndef WORKTRACKERCONTROLLER_H
#define WORKTRACKERCONTROLLER_H

#include "../model/task.h"
#include "../model/workday.h"

#include <QObject>
#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QTimer>

class QDomDocument;
class TaskListModel;
class WorkTracker;
class PreferencesController;
class QTranslator;

/*!
 * \brief Contains the business logic of the WorkTracker application.
 *
 * The `WorkTrackerController` brings the UI and the database together and coordinates
 * user actions in the UI with the data stored in the XML file. The controller doesn't
 * have many methods to interact with. There are a few to set it up (`run()`,
 * `setUi(WorkTracker*)`) and exactly two that perform the task of tracking tasks.
 * `toggleWorkDay()` either starts a new or stops a running workday, based on the current
 * state of the application. The same goes for `toggleTask(QString)`. Both methods emit
 * signals to convey the current status as a result of the `toggle*()` methods.
 */
class WorkTrackerController : public QObject
{
    Q_OBJECT
public:
    /*!
     * Create a new controller with a data source. `dataSource` is owned by client code,
     * the controller only uses it as a reference.
     */
    WorkTrackerController(QDomDocument* dataSource);

    /*!
     * Sets the user interface implementation `ui`. This injects the `TaskListModel`
     * into the UI and stores the reference. Calling `run()` after that shows the UI.
     */
    void setUi(WorkTracker* ui);

    /*!
     * Loads a previously opened but not yet finished workday and shows the UI. Emits the
     * signal `workDayStarted(QDateTime)` if an unfinished workday was loaded.
     */
    void run();

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
    void workDayStarted(QDateTime start);

    /*!
     * Emitted as a result to a call to `toggleWorkDay()` when a workday has been
     * finished.
     *
     * \param now
     * The timestamp of the stop in UTC format.
     */
    void workDayStopped(QDateTime now);

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
    void workTaskStarted(QDateTime now, QString name);

    /*!
     * Emitted as a result to a call to `toggleTask(QString)` when a worktask has been
     * finished.
     *
     * \param now
     * The timestamp of the stop in UTC format.
     *
     * \param name
     * The name of the task.
     */
    void workTaskStopped(QDateTime now, QString name);

    /*!
     * Emitted whenever there is an error processing the `toggle*()` methods.
     *
     * \param text
     * Error description.
     */
    void error(QString text);

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
     * the signal `workDayStarted(QDateTime)` is emitted if a new day has been started or
     * `workDayStopped(QDateTime)` if a running day has been finished.
     */
    void toggleWorkDay();

    /*!
     * Starts or stops a new worktask. The current state is managed by the controller and
     * the signal `workTaskStarted(QDateTime, QString)` is emitted if a new task has been
     * started or `workTaskStopped(QDateTime, QString)` if a running task has been
     * finished.
     */
    void toggleTask(QString name);
    
    /*!
     * Replaces the currently running task with a new one. If the current task is not yet
     * finished then the current time is used.
     * 
     * \param p_task
     * The new task to replace an existing one. It is only accepted if `WorkTask::task()`
     * returns a valid instance. If the start time is missing the current one will be 
     * used.
     */
    void setActiveTask(WorkTask p_task);
    
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

    /*!
     * Shows the editor wizard.
     */
    void showEditor();
    
    /*!
     * Shows the settings dialog.
     */
    void showPreferences();

    /**
     * Installs the language based on the locale.
     *
     * @param p_locale
     * The locale for which to install the language. If the locale doesn't exist then the
     * language is not changed.
     */
    void setLanguage(const QString& p_locale);

private:
    /*!
     * The loaded XML database.
     */
    QDomDocument* m_dataSource;
    /*!
     * The user interface.
     */
    WorkTracker* m_ui;
    /*!
     * The Qt Model View based model as injected into the UI.
     */
    TaskListModel* m_taskListModel;

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
     * Lazily created settings controller and dialog.
     */
    PreferencesController* m_preferencesController;

    /*!
     * Maps the locale to the Qt and the application translation.
     */
    QMap<QString, QPair<QTranslator*, QTranslator*>> m_translations;
    /*!
     * The currently used language. This is needed to remove the currently installed
     * translations when `WorkTrackerController::setLanguage(const QString&)` is called.
     */
    QString m_currentLocale;

    /*!
     * The actual implementation that starts a new workday. Called by `toggleWorkDay()`.
     */
    void startWorkDay(QDateTime now);

    /*!
     * The actual implementation that stops a running workday. Called by
     * `toggleWorkDay()`.
     */
    void stopWorkDay(QDateTime now);

    /*!
     * The actual implementation that starts a new task. Called by `toggleTask(QString)`.
     */
    void startWorkTask(QString name);

    /*!
     * The actual implementation that stops a running task. Called by
     * `toggleTask(QString)`.
     */
    void stopWorkTask(QString name);

    /*!
     * Searches the database for a task by `name` or adds a new one if it can't be found.
     
    Task findOrCreateTaskItem(QString name);*/
    
    /*!
     * Sets up the language and translation related objects. Loads the translation files
     * and sets the system's language or the one the user selected.
     */
    void loadTranslations();
};

#endif // WORKTRACKERCONTROLLER_H
