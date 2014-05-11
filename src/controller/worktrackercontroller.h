/*
 * Copyright 2014 Robert Lohr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WORKTRACKERCONTROLLER_H
#define WORKTRACKERCONTROLLER_H

#include "../model/task.h"
#include "../model/workday.h"

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

    void error(QString text);

public slots:
    void toggleWorkDay();
    void toggleTask(QString name);

private:
    QDomDocument*  m_dataSource;
    WorkTracker*   m_ui;
    TaskListModel* m_taskListModel;

    WorkDay  m_workday;
    WorkTask m_recordingWorkTask;

    bool m_isNewWorkDay;
    bool m_isRecording;

    void startWorkDay(QDateTime now);
    void stopWorkDay(QDateTime now);

    void startWorkTask(QString name);
    void stopWorkTask(QString name);

    Task findOrCreateTaskItem(QString name);
};

#endif // WORKTRACKERCONTROLLER_H
