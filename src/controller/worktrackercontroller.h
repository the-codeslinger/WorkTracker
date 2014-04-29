#ifndef WORKTRACKERCONTROLLER_H
#define WORKTRACKERCONTROLLER_H

#include "../model/task.h"
#include "../model/workday.h"
#include "../model/workdaylist.h"

#include <QString>
#include <QTime>

class QDomDocument;
class TaskListModel;

class WorkTrackerController
{
public:
    WorkTrackerController(QDomDocument* dataSource);

    QDomDocument* dataSource() const;
    TaskListModel* createUiModel() const;

    void startWorkDay();
    void stopWorkDay();
    bool isRunningWorkDay() const;

    void startWorkTask();
    bool stopWorkTask(QString name);

    QString generateSummary() const;

    void close();

private:
    QDomDocument* m_dataSource;

    WorkDay     m_workday;
    WorkDayList m_workdays;

    QTime m_taskStart;
};

#endif // WORKTRACKERCONTROLLER_H
