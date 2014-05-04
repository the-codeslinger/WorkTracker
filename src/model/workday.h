#ifndef WORKDAY_H
#define WORKDAY_H

#include "worktask.h"
#include "xmldata.h"

#include <QDateTime>
#include <QList>
#include <QDomNode>

class QDomDocument;

class WorkDay : public XmlData
{
public:
    WorkDay();
    WorkDay(QDomDocument* dataSource);
    WorkDay(QDomDocument* dataSource, QDomElement node);
    WorkDay(QDomDocument* dataSource, QDateTime start);
    WorkDay(QDomDocument* dataSource, QDateTime start, QDateTime stop);

    QDateTime start() const;
    void setStart(QDateTime start);

    QDateTime stop() const;
    void setStop(QDateTime stop);

    void addTask(WorkTask task);
    void clear();

    QString generateSummary() const;


    static WorkDay findLastOpen(QDomDocument* dataSource);

private:
    QList<WorkTask> m_tasks;

    void createNode(QDateTime start, QDateTime stop);

    QDomNode findTask(QDomNode day, int id) const;

    static WorkDay fromDomNode(QDomElement node, QDomDocument* dataSource);
};

#endif // WORKDAY_H
