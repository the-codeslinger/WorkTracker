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
