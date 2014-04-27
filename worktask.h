#ifndef WORKTASK_H
#define WORKTASK_H

#include "task.h"
#include <QVector>
#include <QTime>
#include <QPair>

class WorkTask
{
public:
    WorkTask();
    WorkTask(Task task, QTime start, QTime stop);

    Task task() const;
    QTime start() const;
    QTime stop() const;

    int totalTime() const;

private:
    Task  m_task;
    QTime m_start;
    QTime m_stop;
};

#endif // WORKTASK_H
