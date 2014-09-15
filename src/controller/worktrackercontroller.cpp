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
#include "../helper.h"
#include "editorcontroller.h"
#include "preferencescontroller.h"

#include <QDomDocument>
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QMapIterator>
#include <QSettings>

static const int TIMER_TIMEOUT = 30 * 1000;

WorkTrackerController::WorkTrackerController(const QDomDocument& dataSource)
    : m_dataSource(dataSource)
    , m_taskListModel(nullptr)
    , m_workday(dataSource)
    , m_recordingWorkTask(dataSource)
    , m_recordingWorkTime(dataSource)
    , m_isNewWorkDay(false)
    , m_isRecording(false)
    , m_preferencesController(nullptr)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    loadTranslations();
}

void
WorkTrackerController::setUi(WorkTracker* ui)
{
    m_ui = ui;
    m_taskListModel = new TaskListModel(m_dataSource, this);
    m_ui->setTaskListModel(m_taskListModel);
    m_ui->setLanguageChecked(m_currentLocale);
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

        m_recordingWorkTask = m_workday.activeWorkTask();
        m_recordingWorkTime = m_recordingWorkTask.activeWorkTime();
        m_isRecording = !m_recordingWorkTime.isNull();
        if (m_isRecording) {
            m_timer.start(TIMER_TIMEOUT);
            emit workTaskStarted(m_recordingWorkTime.start(),
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
WorkTrackerController::startWorkDay(QDateTime p_now)
{
    m_workday = WorkDay(m_dataSource, p_now);

    QDomElement root = m_dataSource.documentElement();
    QDomElement days = root.firstChildElement("workdays");

    QDomNode elem = m_workday.node();
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
    Task task(m_dataSource, name, now.date());
    
    // Always reset before starting a new one.
    m_recordingWorkTime = WorkTime(m_dataSource);
    m_recordingWorkTask = m_workday.workTask(task);
    if (m_recordingWorkTask.isNull()) {
        m_recordingWorkTask = WorkTask(m_dataSource);
        m_recordingWorkTask.setTask(task);
    }

    m_recordingWorkTime.setStart(now);
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
    Task newTask(m_dataSource, name, QDate::currentDate());
    Task recTask = m_recordingWorkTask.task();
    
    if (newTask.id() != recTask.id()) {
        WorkTask otherWorkTask = m_workday.workTask(newTask);
        if (otherWorkTask.isNull()) {
            otherWorkTask = WorkTask(m_dataSource, m_workday.parent(), newTask);
            m_workday.addWorkTask(otherWorkTask);
        }
        
        otherWorkTask.addTime(m_recordingWorkTime);
    }
    
    QDateTime now = QDateTime::currentDateTimeUtc();    
    m_recordingWorkTime.setStop(now);
    // Always reset after it is not needed any more
    m_recordingWorkTask = WorkTask(m_dataSource);
    m_recordingWorkTime = WorkTime(m_dataSource);

    emit workTaskStopped(now, name);
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
WorkTrackerController::showEditor()
{
    EditorController econ(m_dataSource, this);
    econ.run();
}

void 
WorkTrackerController::showPreferences()
{
    if (nullptr == m_preferencesController) {
        m_preferencesController = new PreferencesController(m_ui, this);
        connect(m_preferencesController, &PreferencesController::languageChanged,
                this,                    &WorkTrackerController::setLanguage);
    }
    
    m_preferencesController->run();
}

void
WorkTrackerController::setLanguage(const QString& p_locale)
{
    if (!m_translations.contains(p_locale)) {
        qDebug() << "Language doesn't exist <" << p_locale << ">";
        return;
    }

    auto current     = m_translations.value(m_currentLocale);
    auto translation = m_translations.value(p_locale);

    qApp->removeTranslator(current.first);
    qApp->removeTranslator(current.second);
    
    QLocale::setDefault(QLocale(p_locale));
    
    qApp->installTranslator(translation.first);
    qApp->installTranslator(translation.second);

    m_currentLocale = p_locale;
    
    QSettings settings;
    settings.setValue("Locale", p_locale);
}

void 
WorkTrackerController::loadTranslations()
{
    // These are the languages the application supports
    QStringList locales = { "en_US", "de_DE" };
    
    // Load the user settings. If none is selected (probably after an update) then try
    // the system language. If that is not supported then fall back to English.
    QSettings settings;
    QString appLocale = settings.value("Locale").toString();
    if (appLocale.isEmpty()) {
        appLocale = QLocale().name();
        settings.setValue("Language", appLocale);
    }
    
    if (!locales.contains(appLocale)) {
        appLocale = "en_US";
        settings.setValue("Language", appLocale);
    }
    
    // Now load the translationss and install one
    QString appDir = QApplication::applicationDirPath();
    
    for (const QString& locale : locales) {
        // Load Qt language file first
        QTranslator* qtLang = new QTranslator(this);
        if (!qtLang->load(appDir + "/l10n/qt_" + locale + ".qm")) {
            qDebug() << "Cannot load Qt language file <" << locale << "> use English";
            delete qtLang;
            qtLang = nullptr;
        }
        
        // Now load WorkTracker language file
        QTranslator* appLang = new QTranslator(this);
        if (!appLang->load(appDir + "/l10n/" + locale + ".qm")) {
            qDebug() << "Cannot load WorkTracker language file <" << locale 
                     << "> use English";
            delete appLang;
            appLang = nullptr;
        }
        
        m_translations.insert(locale, qMakePair(qtLang, appLang));
        
        if (locale == appLocale) {
            QLocale::setDefault(QLocale(appLocale));
            qApp->installTranslator(qtLang);
            qApp->installTranslator(appLang);
            m_currentLocale = appLocale;
        }
    }
}

void 
WorkTrackerController::setActiveTask(WorkTask p_task)
{
    if (p_task.isNull() || p_task.task().name().isEmpty()) {
        return; // Sorry pal, can't help ya.
    }
    
    closeCurrentTask();
    
    m_recordingWorkTask = p_task;
    m_recordingWorkTime = p_task.activeWorkTime();
    
    if (m_recordingWorkTime.start().isNull()) {
        m_recordingWorkTime.setStart(QDateTime::currentDateTimeUtc());
    }
    
    emit workTaskStarted(m_recordingWorkTime.start(), m_recordingWorkTask.task().name());
}

void 
WorkTrackerController::closeCurrentTask()
{
    if (m_recordingWorkTask.isNull()) {
        return; // Nothing to do here
    }
    
    // Simply rely on what we already have. This also takes care about sending the 
    // necessary signal to update the ui.
    stopWorkTask(m_recordingWorkTask.task().name());
}
