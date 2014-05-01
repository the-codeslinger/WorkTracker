#ifndef WORKTRACKERCONTROLLER_H
#define WORKTRACKERCONTROLLER_H

#include "../model/task.h"
#include "../model/workday.h"
#include "../model/workdaylist.h"

#include <QObject>
#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>

class QDomDocument;
class TaskListModel;
class WorkTracker;

class WorkTrackerController : public QObject
{
    Q_OBJECT
public:
    WorkTrackerController(QDomDocument* dataSource);

    void setUi(WorkTracker* ui);
    void run();
    void close();

    bool isRecording() const;
    QString generateSummary() const;

signals:
    void workDayStarted(QDateTime start);
    void workDayStopped(QDateTime now);

    void workTaskStarted(QDateTime now, QString name);
    void workTaskStopped(QDateTime now, QString name);

public slots:
    void toggleWorkDay();
    void toggleTask(QString name);

private:
    QDomDocument*  m_dataSource;
    WorkTracker*   m_ui;
    TaskListModel* m_taskListModel;

    WorkDay     m_workday;
    WorkDayList m_workdays;
    WorkTask    m_recordingWorkTask;

    bool m_isNewWorkDay;
    bool m_isRecording;

    void startWorkDay(QDateTime now);
    void stopWorkDay(QDateTime now);

    void startWorkTask(QString name);
    void stopWorkTask(QString name);

    Task findOrCreateTaskItem(QString name);
};

#endif // WORKTRACKERCONTROLLER_H
