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

#ifndef WORKTRACKER_H
#define WORKTRACKER_H

#include "model/ui/tasklistmodel.h"

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QDateTime>

namespace Ui {
    class WorkTracker;
}

class QLabel;
class WorkTrackerController;

class WorkTracker : public QMainWindow
{
    Q_OBJECT
public:
    explicit WorkTracker(WorkTrackerController* controller, QWidget *parent = 0);
    ~WorkTracker();

    void setTaskListModel(TaskListModel* model);

public slots:
    void workDayStarted(QDateTime now);
    void workDayStopped(QDateTime now);

    void workTaskStarted(QDateTime now, QString name);
    void workTaskStopped(QDateTime now, QString name);

private slots:
    void taskSelected();
    void showAnimationFinished();
    void showSummary();
    void hideSummary();
    void showInput();
    void hideInput();

private:
    Ui::WorkTracker* ui;
    TaskListModel*   m_taskModel;
    QLabel*          m_statusDay;
    QLabel*          m_statusRecording;

    QWidget*           m_animatedWidget;
    QPropertyAnimation m_showAnimation;
    QPropertyAnimation m_hideAnimation;

    WorkTrackerController* m_controller;

    int m_collapsedHeight;
};

#endif // WORKTRACKER_H
