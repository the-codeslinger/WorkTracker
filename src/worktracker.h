#ifndef WORKTRACKER_H
#define WORKTRACKER_H

#include "model/ui/tasklistmodel.h"
#include "model/workday.h"
#include "model/workdaylist.h"
#include "controller/worktrackercontroller.h"

#include <QMainWindow>
#include <QDomDocument>
#include <QPropertyAnimation>

namespace Ui {
    class WorkTracker;
}

class QLabel;

class WorkTracker : public QMainWindow
{
    Q_OBJECT

public:
    explicit WorkTracker(QWidget *parent = 0);
    ~WorkTracker();

public slots:
    void taskSelected();
    void toggleTask();
    void toggleWorkday();
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

    QDomDocument m_xml;
    WorkTrackerController m_controller;

    WorkDay        m_workday;
    WorkDayList    m_workdays;

    bool m_isRecording;
    bool m_isNewWorkDay;
    QTime m_taskStart;

    void toggleInput();
};

#endif // WORKTRACKER_H
