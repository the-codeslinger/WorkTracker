#include "workday.h"

#include <QDomDocument>
#include <QTextStream>
#include <QMapIterator>

WorkDay::WorkDay()
{
}

WorkDay::WorkDay(QDate day)
    : m_day(day)
{
}

QDate
WorkDay::day() const
{
    return m_day;
}

void
WorkDay::setDay(QDate day)
{
    m_day = day;
}

void
WorkDay::addTask(WorkTask task)
{
    m_tasks.append(task);
}

void
WorkDay::clear()
{
    m_day.setDate(0, 0, 0);
    m_tasks.clear();
}

bool
WorkDay::isNull() const
{
    return m_day.isNull() && m_tasks.isEmpty();
}

QDomElement
WorkDay::createElement(QDomDocument* dataSource) const
{
    if (m_day.isNull()) {
        return QDomElement();
    }

    QDomElement day = dataSource->createElement("day");
    day.setAttribute("date", m_day.toString(Qt::ISODate));

    for (WorkTask task : m_tasks) {
        QDomElement taskElem = findTask(&day, task.task().id());
        if (taskElem.isNull()) {
            taskElem = dataSource->createElement("task");
            taskElem.setAttribute("id", task.task().id());
            day.appendChild(taskElem);
        }

        QDomElement timeElem = dataSource->createElement("time");
        timeElem.setAttribute("start", task.start().toString(Qt::TextDate));
        timeElem.setAttribute("stop",  task.stop().toString(Qt::TextDate));

        taskElem.appendChild(timeElem);
    }

    return day;
}

QDomElement
WorkDay::findTask(QDomElement* day, int id) const
{
    QDomNodeList tasks = day->childNodes();
    if (tasks.isEmpty()) {
        return QDomElement();
    }

    int count = tasks.count();
    for (int c = 0; c < count; c++) {
        QDomNode node = tasks.item(c);
        if (!node.isNull() && node.isElement()) {
            QDomNamedNodeMap attrs = node.attributes();
            QDomAttr idAttr = attrs.namedItem("id").toAttr();
            if (!idAttr.isNull()) {
                bool ok = false;
                int foundId = idAttr.value().toInt(&ok);
                if (ok) {
                    if (foundId == id) {
                        return node.toElement();
                    }
                }
            }
        }
    }

    return QDomElement();
}

QString
WorkDay::generateSummary() const
{
    QString value;
    QTextStream html(&value);
    html << "<html>"
         << "<head><title>WorkTracker Summary</title></head>"
         << "<body><ul>";

    QMap<QString, int> durations;
    for (WorkTask task : m_tasks) {
        QMap<QString, int>::iterator found = durations.find(task.task().name());
        if (found == durations.end()) {
            durations.insert(task.task().name(), task.totalTime());
        }
        else {
            int& totalTime = found.value();
            totalTime += task.totalTime();
        }
    }

    QMapIterator<QString, int> iter(durations);
    while (iter.hasNext()) {
        auto item = iter.next();

        float hours = item.value() / 60.0f;

        html << "<li><i>" << item.key() << "</i>: ";
        html << hours << " h (" << item.value() << " min)";
        html << "</li>";
    }

    html << "</ul></body></html>";

    return value;
}

WorkDay
WorkDay::fromDomNode(QDomNode* node, QDomDocument* dataSource)
{
    QDomNamedNodeMap attributes = node->attributes();
    QDomAttr dateAttr = attributes.namedItem("date").toAttr();
    if (dateAttr.isNull()) {
        return WorkDay();
    }

    QDate date = QDate::fromString(dateAttr.value(), Qt::ISODate);
    WorkDay day(date);

    QDomNodeList children = node->childNodes();

    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode taskNode = children.item(c);
        if (!taskNode.isNull()) {
            QList<WorkTask> tasks = WorkTask::fromDomNode(&taskNode, dataSource);
            day.m_tasks.append(tasks);
        }
    }

    return day;
}
