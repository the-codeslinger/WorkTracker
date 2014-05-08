#include "worktask.h"
#include "task.h"
//#include "tasklist.h"

#include <QDomNode>
#include <QDomNodeList>

WorkTask::WorkTask()
    : XmlData(nullptr)
{
}

WorkTask::WorkTask(QDomDocument* dataSource)
    : XmlData(dataSource)
    , m_task(dataSource)
{
}

WorkTask::WorkTask(QDomDocument* dataSource, Task task, QDateTime start, QDateTime stop)
    : XmlData(dataSource)
    , m_task(task)
    , m_start(start)
    , m_stop(stop)
{
}

Task
WorkTask::task() const
{
    return m_task;
}

void
WorkTask::setTask(Task task)
{
    m_task = task;
}

QDateTime
WorkTask::start() const
{
    return m_start;
}

void
WorkTask::setStart(QDateTime start)
{
    m_start = start;
}

QDateTime
WorkTask::stop() const
{
    return m_stop;
}

void
WorkTask::setStop(QDateTime stop)
{
    m_stop = stop;
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

void
WorkTask::clear()
{
    m_task.clear();
    m_start = QDateTime();
    m_stop  = QDateTime();
}

QList<WorkTask>
WorkTask::fromDomNode(QDomNode* node, QDomDocument* dataSource)
{
    QDomNamedNodeMap attributes = node->attributes();

    QDomNode attrNode = attributes.namedItem("id");
    int id = idFromAttr(&attrNode);
    if (Task::invalidId == id) {
        return QList<WorkTask>();
    }

    Task task = Task::get(id, dataSource);
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

        attrNode = attributes.namedItem("start");
        QDateTime start = timestampFromAttr(&attrNode);

        attrNode = attributes.namedItem("stop");
        QDateTime stop  = timestampFromAttr(&attrNode);

        if (start.isValid() && stop.isValid()) {
            workTasks.append(WorkTask(dataSource, task, start, stop));
        }
    }

    return workTasks;
}

QDateTime
WorkTask::timestampFromAttr(QDomNode* attr)
{
    QDomAttr a = attr->toAttr();
    if (a.isNull()) {
        return QDateTime();
    }

    return QDateTime::fromString(a.value(), Qt::ISODate);
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
