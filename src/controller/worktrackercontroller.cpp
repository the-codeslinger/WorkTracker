#include "worktrackercontroller.h"
#include "../model/ui/tasklistmodel.h"
#include "../worktracker.h"

#include <QDomDocument>
#include <QDateTime>
#include <QDebug>

WorkTrackerController::WorkTrackerController(QDomDocument* dataSource)
    : m_dataSource(dataSource)
    , m_taskListModel(nullptr)
    , m_workdays(dataSource)
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
    m_workday      = m_workdays.findToday();
    m_isNewWorkDay = !m_workday.isNull();
    if (m_isNewWorkDay) {
        emit workDayStarted(m_workday.started());
    }
    m_ui->show();
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
    // If there's an ongoing workday close it and save it
    // TODO Add methods to WorkDayList (or replace it as was the case with TaskList) to
    //      figure out whether the current day has already been added.
    //      For now one has to explicitly close off the current day.
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
    m_workday.clear();
    m_workday.setStarted(now);

    emit workDayStarted(now);
}

void
WorkTrackerController::stopWorkDay(QDateTime now)
{
    if (!m_workday.isNull()) {
        m_workdays.addWorkDay(m_workday);
    }

    emit workDayStopped(now);
}

void
WorkTrackerController::startWorkTask(QString name)
{
    // Always reset before starting a new one
    m_recordingWorkTask = WorkTask();

    QDateTime now = QDateTime::currentDateTimeUtc();
    m_recordingWorkTask.setStart(now);
    if (!name.isEmpty()) {
        Task task = findOrCreateTaskItem(name);
        m_recordingWorkTask.setTask(task);
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
    m_workday.addTask(m_recordingWorkTask);
    m_taskListModel->itemAppended();
    // Always reset after it is not needed any more
    m_recordingWorkTask = WorkTask();

    emit workTaskStopped(now, name);
}

Task
WorkTrackerController::findOrCreateTaskItem(QString name)
{
    Task taskItem = Task::findByName(name, m_dataSource);
    if (taskItem.isNull()) {
        // Create a new one
        taskItem = Task(Task::count(m_dataSource), name, QDate::currentDate());
        QDomElement taskElem = taskItem.createElement(m_dataSource);
        if (taskElem.isNull()) {
            qDebug() << "Could not create a new task XML element";
            return Task();
        }

        QDomElement root   = m_dataSource->documentElement();
        QDomElement tasks  = root.firstChildElement("tasks");
        QDomNode    result = tasks.appendChild(taskElem);
        if (result.isNull()) {
            qDebug() << "Could append task XML element to tasks element";
            return Task();
        }
    }

    return taskItem;
}
