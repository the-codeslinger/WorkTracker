#ifndef WORKTRACKERCONTROLLER_H
#define WORKTRACKERCONTROLLER_H

#include "../model/task.h"

#include <QString>

class QDomDocument;

class WorkTrackerController
{
public:
    WorkTrackerController();
    void setDataSource(QDomDocument* dataSource);

    bool addTask(QString name);

private:
    QDomDocument* m_dataSource;
};

#endif // WORKTRACKERCONTROLLER_H
