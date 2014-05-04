#include "task.h"

#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>

int Task::invalidId = -1;

Task::Task()
    : XmlData(nullptr)
{
}

Task::Task(QDomDocument* dataSource)
    : XmlData(dataSource)
{
    createNode(Task::invalidId, "", QDate());
}

Task::Task(QDomDocument* dataSource, QString name, QDate lastUsed)
    : XmlData(dataSource)
{
    createNode(Task::invalidId, name, lastUsed);
}

Task::Task(QDomDocument* dataSource, int id, QString name, QDate lastUsed)
    : XmlData(dataSource)
{
    createNode(id, name, lastUsed);
}

Task::Task(QDomDocument* dataSource, QDomElement node)
    : XmlData(dataSource, node)
{
}

Task::Task(const Task& other)
    : XmlData(other)
{
}

int
Task::id() const
{
    // Safe to not check conversion result as this is done in attributeValue()
    return attributeValue("id").toInt();
}

void
Task::setId(int id)
{
    addAttribute("id", Task::invalidId != id ? QString::number(id) : "");
}

QString
Task::name() const
{
    return attributeValue("name").toString();
}

void
Task::setName(const QString& name)
{
    addAttribute("name", name);
}

QDate
Task::lastUsed() const
{
    return attributeValue("last_used").toDate();
}

void
Task::setLastUsed(const QDate& lastUsed)
{
    addAttribute("last_used", lastUsed.toString(Qt::ISODate));
}

void
Task::createNode(int id, QString name, QDate lastUsed)
{
    m_node = m_dataSource->createElement("item");
    addAttribute("id",        Task::invalidId != id ? QString::number(id) : "");
    addAttribute("last_used", lastUsed.toString(Qt::ISODate));
    addAttribute("name",      name);
}

QVariant
Task::attributeValue(QString name) const
{
    QVariant value = XmlData::attributeValue(name);

    if ("id" == name) {
        bool ok = false;
        int id = value.toInt(&ok);
        if (!ok) {
            id = Task::invalidId;
        }
        return QVariant(id);
    }

    if ("name" == name) {
        return value;
    }

    if ("last_used" == name) {
        QDate date = QDate::fromString(value.toString(), Qt::ISODate);
        return QVariant(date);
    }

    // Impossible
    return QVariant();
}

/* * * * * * * * * * * * static methods * * * * * * * * * * * */

Task
Task::fromDomNode(QDomElement node, QDomDocument* dataSource)
{
    return Task(dataSource, node);
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
        if (node.isNull() || !node.isElement()) {
            continue;
        }

        tasks.append(Task::fromDomNode(node.toElement(), dataSource));
    }

    return tasks;
}

int
Task::count(QDomDocument* dataSource)
{
    QDomNodeList children = getTaskNodes(dataSource);
    return children.count();
}

Task
Task::findTask(QDomDocument* dataSource, std::function<bool(Task)> predicate)
{
    QDomNodeList children = getTaskNodes(dataSource);
    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode node = children.item(c);
        if (node.isNull() || !node.isElement()) {
            continue;
        }

        Task item = Task::fromDomNode(node.toElement(), dataSource);
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
