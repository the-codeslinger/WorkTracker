#ifndef WORKTRACKER_H
#define WORKTRACKER_H

#include "tasklistmodel.h"
#include "workday.h"
#include "workdaylist.h"

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
    void showAnimationFinished();
    void startWorkday();
    void showSummary();

private:
    Ui::WorkTracker *ui;
    QLabel* m_statusDay;
    QLabel* m_statusRecording;

    QDomDocument m_xml;

    TaskList       m_tasks;
    TaskListModel* m_taskModel;
    WorkDay        m_workday;
    WorkDayList    m_workdays;

    bool m_isRecording;
    QTime m_taskStart;

    QPropertyAnimation showAnimation;
    QPropertyAnimation hideAnimation;

    void toggleInput();
};

#endif // WORKTRACKER_H
