#include "worktask.h"
#include "task.h"
#include "tasklist.h"

#include <QDomNode>
#include <QDomNodeList>


WorkTask::WorkTask()
{
}

WorkTask::WorkTask(Task task, QTime start, QTime stop)
    : m_task(task)
    , m_start(start)
    , m_stop(stop)
{
}

Task
WorkTask::task() const
{
    return m_task;
}

QTime
WorkTask::start() const
{
    return m_start;
}

QTime
WorkTask::stop() const
{
    return m_stop;
}

int
WorkTask::totalTime() const
{
    return m_start.secsTo(m_stop) / 60;
}

bool
WorkTask::isNull() const
{
    return m_task.isNull();
}

QList<WorkTask>
WorkTask::fromDomNode(QDomNode* node, QDomDocument* dataSource)
{
    QDomNamedNodeMap attributes = node->attributes();
    int id = idFromAttr(&attributes.namedItem("id"));
    if (Task::invalidId == id) {
        return QList<WorkTask>();
    }

    TaskList tasks(dataSource);
    Task task = tasks.itemById(id);
    if (task.isNull()) {
        return QList<WorkTask>();
    }

    // Fetch and add the time objects
    QDomNodeList children = node->childNodes();

    QList<WorkTask> workTasks;

    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode timeNode = children.item(c);
        attributes = timeNode.attributes();

        QTime start = timeFromAttr(&attributes.namedItem("start"));
        QTime stop  = timeFromAttr(&attributes.namedItem("stop"));

        if (start.isValid() && stop.isValid()) {
            workTasks.append(WorkTask(task, start, stop));
        }
    }

    return workTasks;
}

QTime
WorkTask::timeFromAttr(QDomNode* attr)
{
    QDomAttr a = attr->toAttr();
    if (a.isNull()) {
        return QTime();
    }

    return QTime::fromString(a.value(), Qt::TextDate);
}


int
WorkTask::idFromAttr(QDomNode* attr)
{
    QDomAttr a = attr->toAttr();
    if (a.isNull()) {
        return Task::invalidId;
    }

    bool ok = false;
    int id = a.value().toInt(&ok);
    if (!ok) {
        return Task::invalidId;
    }

    return id;
}
