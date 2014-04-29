#ifndef WORKTRACKER_H
#define WORKTRACKER_H

#include "model/ui/tasklistmodel.h"

#include <QMainWindow>
#include <QPropertyAnimation>

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

public slots:
    void taskSelected();
    void toggleTask();
    void toggleWorkday(bool startNewWorkDay = true);
    void showAnimationFinished();
    void showSummary();
    void hideSummary();

private:
    Ui::WorkTracker* ui;
    TaskListModel*   m_taskModel;
    QLabel*          m_statusDay;
    QLabel*          m_statusRecording;

    QWidget*           m_animatedWidget;
    QPropertyAnimation m_showAnimation;
    QPropertyAnimation m_hideAnimation;

    WorkTrackerController* m_controller;

    bool m_isRecording;
    bool m_isNewWorkDay;

    void toggleInput();
};

#endif // WORKTRACKER_H
