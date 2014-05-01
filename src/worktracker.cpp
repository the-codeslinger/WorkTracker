#include "worktracker.h"
#include "ui_worktracker.h"
#include "model/worktask.h"
#include "model/task.h"
#include "controller/worktrackercontroller.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QCompleter>
#include <QPropertyAnimation>
#include <QSize>
#include <QTime>
#include <QDateTime>
#include <QLabel>
#include <QTextEdit>
#include <QDir>

WorkTracker::WorkTracker(WorkTrackerController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkTracker)
    , m_isRecording(false)
    , m_isNewWorkDay(false)
    , m_controller(controller)
{
    ui->setupUi(this);

    m_showAnimation.setTargetObject(this);
    m_showAnimation.setPropertyName("size");
    m_showAnimation.setDuration(200);

    m_hideAnimation.setTargetObject(this);
    m_hideAnimation.setPropertyName("size");
    m_hideAnimation.setDuration(200);

    ui->frame->setVisible(false);
    ui->textEdit->setVisible(false);

    m_statusDay       = new QLabel(tr("Waiting to start a new workday"), this);
    m_statusRecording = new QLabel(tr("Not recording anything yet"), this);
    ui->statusBar->addWidget(m_statusDay, 1);
    ui->statusBar->addWidget(m_statusRecording, 1);

    // Capture the current width which is set by the designer. Then we resize the window
    // to only take up as much space as is needed with the edit field and text edit not
    // being visible. Since this also changes the width we restore it using the value
    // saved earliert. Voila, our UI looks correct.
    int width = this->width();
    this->adjustSize();
    this->resize(width, this->height());


    // For the completer
    m_taskModel = m_controller->createUiModel();
    m_taskModel->setParent(this);

    QCompleter *completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setModel(m_taskModel);
    ui->tasksEdit->setCompleter(completer);

    if (m_controller->isRunningWorkDay()) {
        toggleWorkday(false);
    }

    connect(ui->selectTaskButton, SIGNAL(clicked()),  this, SLOT(taskSelected()));
    connect(ui->taskButton,       SIGNAL(clicked()),  this, SLOT(toggleTask()));
    connect(ui->workdayButton,    SIGNAL(clicked()),  this, SLOT(toggleWorkday()));
    connect(ui->summaryButton,    SIGNAL(clicked()),  this, SLOT(showSummary()));
    connect(&m_showAnimation,     SIGNAL(finished()), this, SLOT(showAnimationFinished()));
}

WorkTracker::~WorkTracker()
{
    delete ui;
}

void
WorkTracker::taskSelected()
{
    QString taskName = ui->tasksEdit->text();
    if (taskName.isEmpty()) {
        ui->statusBar->showMessage(tr("You must enter a task description"), 3000);
        return;
    }

    toggleInput();

    if (m_controller->stopWorkTask(taskName)) {
        m_taskModel->itemAppended();
    }

    ui->taskButton->setEnabled(true);
    ui->summaryButton->setEnabled(true);
}

void
WorkTracker::toggleTask()
{
    QTime now = QTime::currentTime();

    if (m_isRecording) {
        ui->taskButton->setText(tr("Start &Task"));
        ui->taskButton->setEnabled(false);
        ui->summaryButton->setEnabled(false);
        toggleInput();

        m_statusRecording->setText(tr("Task stopped at %1")
                                   .arg(now.toString(Qt::TextDate)));
    }
    else {
        hideSummary();
        ui->workdayButton->setEnabled(false);

        ui->taskButton->setText(tr("Stop &Task"));
        m_controller->startWorkTask();

        m_statusRecording->setText(tr("Task started at %1")
                                   .arg(now.toString(Qt::TextDate)));
    }

    m_isRecording = !m_isRecording;
}

void
WorkTracker::toggleInput()
{
    int height = ui->frame->height();

    if (!m_isRecording) {
        ui->workdayButton->setEnabled(true);

        // Hide the frame and shrink the window
        ui->frame->setVisible(false);

        QSize size = this->size();
        m_hideAnimation.setStartValue(size);

        size.setHeight(size.height() - height);
        m_hideAnimation.setEndValue(size);
        m_hideAnimation.start();
    }
    else {
        // Expand the window so there's room to display the frame for the task input.
        QSize size = this->size();
        m_showAnimation.setStartValue(size);

        size.setHeight(size.height() + height);
        m_showAnimation.setEndValue(size);
        m_showAnimation.start();
        m_animatedWidget = ui->frame;
    }
}

void
WorkTracker::showAnimationFinished()
{
    // The frame can only be shown once there is enough room for it. Otherwise Qt would
    // just create space for it on its own and the animation would add more space on top
    // of that, resulting in a too large window.
    m_animatedWidget->setVisible(true);
}

void
WorkTracker::toggleWorkday(bool startNewWorkDay)
{
    QString date = QDateTime::currentDateTime().toString(Qt::TextDate);

    if (!m_isNewWorkDay) {
        // Start Workday was clicked
        hideSummary();

        if (startNewWorkDay) {
            m_controller->startWorkDay();
        }

        m_statusDay->setText(tr("Work started at %1").arg(date));

        ui->workdayButton->setText(tr("Stop Workday"));
        ui->taskButton->setEnabled(true);
    }
    else {
        // Stop Workday was clicked
        m_statusDay->setText(tr("Work stopped at %1").arg(date));

        ui->workdayButton->setText(tr("Start New Workday"));
        ui->taskButton->setEnabled(false);
        ui->summaryButton->setEnabled(!ui->textEdit->isVisible());

        m_controller->stopWorkDay();
    }

    m_isNewWorkDay = !m_isNewWorkDay;
}

void
WorkTracker::showSummary()
{
    ui->summaryButton->setEnabled(false);

    if (!ui->textEdit->isVisible()) {
        QString html = m_controller->generateSummary();
        ui->textEdit->setHtml(html);

        QSize size = this->size();
        m_showAnimation.setStartValue(size);

        size.setHeight(size.height() + ui->textEdit->height());
        m_showAnimation.setEndValue(size);
        m_showAnimation.start();
        m_animatedWidget = ui->textEdit;
    }
}

void
WorkTracker::hideSummary()
{
    if (ui->textEdit->isVisible()) {
        ui->textEdit->setVisible(false);

        QSize size = this->size();
        m_hideAnimation.setStartValue(size);

        size.setHeight(size.height() - ui->textEdit->height());
        m_hideAnimation.setEndValue(size);
        m_hideAnimation.start();
    }
}
