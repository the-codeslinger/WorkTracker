#include "worktask.h"

WorkTask::WorkTask()
{
}

WorkTask::WorkTask(Task task, QTime start, QTime stop)
    : m_task(task)
    , m_start(start)
    , m_stop(stop)
{
}

Task
WorkTask::task() const
{
    return m_task;
}

QTime
WorkTask::start() const
{
    return m_start;
}

QTime
WorkTask::stop() const
{
    return m_stop;
}

int
WorkTask::totalTime() const
{
    return m_start.secsTo(m_stop) / 60;
}
