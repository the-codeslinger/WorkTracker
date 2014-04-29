#include "worktrackercontroller.h"
#include "../model/ui/tasklistmodel.h"

#include <QDomDocument>
#include <QDate>

WorkTrackerController::WorkTrackerController(QDomDocument* dataSource)
    : m_dataSource(dataSource)
    , m_workdays(dataSource)
{
    m_workday = m_workdays.findToday();
}

QDomDocument*
WorkTrackerController::dataSource() const
{
    return m_dataSource;
}

TaskListModel*
WorkTrackerController::createUiModel() const
{
    return new TaskListModel(m_dataSource);
}

void
WorkTrackerController::startWorkDay()
{
    m_workday.clear();
    m_workday.setDay(QDate::currentDate());
}

void
WorkTrackerController::stopWorkDay()
{
    if (!m_workday.isNull()) {
        m_workdays.addWorkDay(m_workday);
    }
}

bool
WorkTrackerController::isRunningWorkDay() const
{
    return !m_workday.isNull();
}

void
WorkTrackerController::startWorkTask()
{
    m_taskStart = QTime::currentTime();
}

bool
WorkTrackerController::stopWorkTask(QString name)
{
    Task found = Task::findByName(name, m_dataSource);
    if (found.isNull()) {
        // Create a new one
        found = Task(Task::count(m_dataSource), name, QDate::currentDate());
        QDomElement taskElem = found.createElement(m_dataSource);
        if (taskElem.isNull()) {
            return false;
        }

        QDomElement root   = m_dataSource->documentElement();
        QDomElement tasks  = root.firstChildElement("tasks");
        QDomNode    result = tasks.appendChild(taskElem);
        if (result.isNull()) {
            return false;
        }
    }

    m_workday.addTask(WorkTask(found, m_taskStart, QTime::currentTime()));
    m_taskStart.setHMS(0, 0, 0);
    return true;
}

QString
WorkTrackerController::generateSummary() const
{
    return m_workday.generateSummary();
}

void
WorkTrackerController::close()
{
    // If there's an ongoing workday close it and save it
    // TODO Add methods to WorkDayList (or replace it as was the case with TaskList) to
    //      figure out whether the current day has already been added.
    //      For now one has to explicitly close off the current day.
}
