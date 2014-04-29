#ifndef WORKTASK_H
#define WORKTASK_H

#include "task.h"
#include <QList>
#include <QTime>
#include <QPair>

class QDomNode;
class QDomDocument;

class WorkTask
{
public:
    WorkTask();
    WorkTask(Task task, QTime start, QTime stop);

    Task task() const;
    QTime start() const;
    QTime stop() const;

    int totalTime() const;

    bool isNull() const;
    void clear();

private:
    Task  m_task;
    QTime m_start;
    QTime m_stop;

    friend class WorkDay;

    static QList<WorkTask> fromDomNode(QDomNode* node, QDomDocument* dataSource);
    static int idFromAttr(QDomNode* attr);
    static QTime timeFromAttr(QDomNode* attr);
};

#endif // WORKTASK_H
