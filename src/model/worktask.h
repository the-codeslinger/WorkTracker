#ifndef WORKTASK_H
#define WORKTASK_H

#include "task.h"
#include <QList>
#include <QDateTime>
#include <QPair>

class QDomNode;
class QDomDocument;

class WorkTask
{
public:
    WorkTask();
    WorkTask(Task task, QDateTime start, QDateTime stop);

    Task task() const;
    void setTask(Task task);

    QDateTime start() const;
    void setStart(QDateTime start);

    QDateTime stop() const;
    void setStop(QDateTime stop);

    int totalTime() const;

    bool isNull() const;
    void clear();

private:
    Task      m_task;
    QDateTime m_start;
    QDateTime m_stop;

    friend class WorkDay;

    static QList<WorkTask> fromDomNode(QDomNode* node, QDomDocument* dataSource);
    static int idFromAttr(QDomNode* attr);
    static QDateTime timestampFromAttr(QDomNode* attr);
};

#endif // WORKTASK_H
