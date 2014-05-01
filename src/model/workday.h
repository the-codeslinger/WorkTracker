#ifndef WORKDAY_H
#define WORKDAY_H

#include "worktask.h"

#include <QDateTime>
#include <QList>
#include <QDomElement>

class QDomDocument;

class WorkDay
{
public:
    WorkDay();
    WorkDay(QDateTime day);

    QDateTime started() const;
    void setStarted(QDateTime started);

    void addTask(WorkTask task);
    void clear();
    bool isNull() const;

    QString generateSummary() const;

private:
    QDateTime m_started;
    QList<WorkTask> m_tasks;

    friend class WorkDayList;

    QDomElement createElement(QDomDocument* dataSource) const;
    QDomElement findTask(QDomElement* day, int id) const;

    static WorkDay fromDomNode(QDomNode* node, QDomDocument* dataSource);
};

#endif // WORKDAY_H
