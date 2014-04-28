#include "task.h"

#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>

int Task::invalidId = -1;

Task::Task()
    : m_id(Task::invalidId)
{
}

Task::Task(QString name, QDate lastUsed)
    : m_id(Task::invalidId)
    , m_name(name)
    , m_lastUsed(lastUsed)
{
}

Task::Task(int id, QString name, QDate lastUsed)
    : m_id(id)
    , m_name(name)
    , m_lastUsed(lastUsed)
{
}

int
Task::id() const
{
    return m_id;
}

void
Task::setId(int id)
{
    m_id = id;
}

QString
Task::name() const
{
    return m_name;
}

void
Task::setName(const QString& name)
{
    m_name = name;
}

QDate
Task::lastUsed() const
{
    return m_lastUsed;
}

void
Task::setLastUsed(const QDate& lastUsed)
{
    m_lastUsed = lastUsed;
}

bool
Task::isNull() const
{
    return Task::invalidId == m_id;
}

QDomElement
Task::createElement(int id, QDomDocument* dataSource) const
{
    QDomElement elem = dataSource->createElement("item");
    elem.setAttribute("id",        id);
    elem.setAttribute("last_used", m_lastUsed.toString(Qt::ISODate));
    elem.setAttribute("name",      m_name);
    return elem;
}

/* * * * * * * * * * * * static methods * * * * * * * * * * * */

Task
Task::fromDomNode(QDomNode* node)
{
    QDomNamedNodeMap attrs = node->attributes();

    int id = idFromAttr(&attrs.namedItem("id"));
    QString name = nameFromAttr(&attrs.namedItem("name"));
    QDate lastUsed = lastUsedFromAttr(&attrs.namedItem("last_used"));


    return Task(id, name, lastUsed);
}

Task
Task::get(int id, QDomDocument* dataSource)
{
    return findTask(dataSource, [id](Task task) {
        return task.id() == id;
    });
}


Task
Task::findByName(QString name, QDomDocument* dataSource)
{
    return findTask(dataSource, [name](Task task) {
        return task.name() == name;
    });
}

QList<Task>
Task::list(QDomDocument* dataSource)
{
    QList<Task> tasks;

    QDomNodeList children = getTaskNodes(dataSource);
    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode node = children.item(c);
        if (node.isNull()) {
            continue;
        }

        tasks.append(Task::fromDomNode(&node));
    }

    return tasks;
}

int
Task::count(QDomDocument* dataSource)
{
    QDomNodeList children = getTaskNodes(dataSource);
    return children.count();
}

int
Task::idFromAttr(QDomNode* attr)
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

QString
Task::nameFromAttr(QDomNode* attr)
{
    QDomAttr a = attr->toAttr();
    if (a.isNull()) {
        return QString();
    }

    return a.value();
}

QDate
Task::lastUsedFromAttr(QDomNode* attr)
{
    QDomAttr a = attr->toAttr();
    if (a.isNull()) {
        return QDate();
    }

    return QDate::fromString(a.value(), Qt::ISODate);
}

Task
Task::findTask(QDomDocument* dataSource, std::function<bool(Task)> predicate)
{
    QDomNodeList children = getTaskNodes(dataSource);
    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode node = children.item(c);
        if (node.isNull()) {
            continue;
        }

        Task item = Task::fromDomNode(&node);
        if (predicate(item)) {
            return item;
        }
    }

    return Task();
}

QDomNodeList
Task::getTaskNodes(QDomDocument* dataSource)
{
    QDomElement root  = dataSource->documentElement();
    if (root.isNull()) {
        return QDomNodeList();
    }

    QDomNode tasks = root.firstChildElement("tasks");
    if (tasks.isNull()) {
        return QDomNodeList();
    }

    return tasks.childNodes();
}
