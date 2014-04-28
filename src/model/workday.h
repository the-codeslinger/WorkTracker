#ifndef WORKDAY_H
#define WORKDAY_H

#include "worktask.h"

#include <QDate>
#include <QList>
#include <QDomElement>

class QDomDocument;

class WorkDay
{
public:
    WorkDay();
    WorkDay(QDate day);

    QDate day() const;
    void setDay(QDate day);

    void addTask(WorkTask task);
    void clear();
    bool isNull() const;

    QString generateSummary() const;

private:
    QDate m_day;
    QList<WorkTask> m_tasks;

    friend class WorkDayList;

    QDomElement createElement(QDomDocument* dataSource) const;
    QDomElement findTask(QDomElement* day, int id) const;

    static WorkDay fromDomNode(QDomNode* node, QDomDocument* dataSource);
};

#endif // WORKDAY_H
