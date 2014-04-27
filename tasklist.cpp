#include "tasklist.h"

#include <QDomDocument>
#include <QDomNodeList>
#include <QDomNamedNodeMap>

TaskList::TaskList(QDomDocument* dataSource)
    : m_dataSource(nullptr)
{
    setDataSource(dataSource);
}

void
TaskList::setDataSource(QDomDocument* dataSource)
{
    if (nullptr != dataSource) {
        m_dataSource = dataSource;
        QDomElement root = m_dataSource->documentElement();
        m_tasks = root.firstChildElement("tasks");
    }
}

int
TaskList::count() const
{
    if (m_tasks.isNull()) {
        return 0;
    }

    return m_tasks.childNodes().count();
}

Task
TaskList::itemById(int id) const
{
    return findTask([id](Task task) {
        return task.id() == id;
    });
}

Task
TaskList::itemByName(QString name) const
{
    return findTask([name](Task task) {
        return task.name() == name;
    });
}

bool
TaskList::addItem(Task item)
{
    QDomNode result = m_tasks.appendChild(item.createElement(count(), m_dataSource));
    return !result.isNull();
}

Task
TaskList::findTask(std::function<bool(Task)> predicate) const
{
    QDomNodeList children = m_tasks.childNodes();

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
