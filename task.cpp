#include "task.h"

#include <QDomDocument>

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
Task::createElement(int id, QDomDocument* dataSource)
{
    QDomElement elem = dataSource->createElement("item");
    elem.setAttribute("id",        id);
    elem.setAttribute("last_used", m_lastUsed.toString(Qt::ISODate));
    elem.setAttribute("name",      m_name);
    return elem;
}

Task
Task::fromDomNode(QDomNode* node)
{
    QDomNamedNodeMap attrs = node->attributes();

    int id = idFromAttr(&attrs.namedItem("id"));
    QString name = nameFromAttr(&attrs.namedItem("name"));
    QDate lastUsed = lastUsedFromAttr(&attrs.namedItem("last_used"));


    return Task(id, name, lastUsed);
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
