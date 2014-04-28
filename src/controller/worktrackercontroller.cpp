#include "worktrackercontroller.h"

#include <QDomDocument>
#include <QDate>

WorkTrackerController::WorkTrackerController()
{
}

void
WorkTrackerController::setDataSource(QDomDocument* dataSource)
{
    m_dataSource = dataSource;
}

bool
WorkTrackerController::addTask(QString name)
{
    Task found = Task::findByName(name, m_dataSource);
    if (!found.isNull()) {
        // If we already know the value we can quit this
        return true;
    }

    Task item(name, QDate::currentDate());
    QDomElement taskElem = item.createElement(Task::count(m_dataSource), m_dataSource);
    if (taskElem.isNull()) {
        return false;
    }

    QDomElement root   = m_dataSource->documentElement();
    QDomElement tasks  = root.firstChildElement("tasks");
    QDomNode    result = tasks.appendChild(taskElem);
    return !result.isNull();
}
