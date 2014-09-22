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

#include "worktracker.h"
#include "aboutdialog.h"
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
#include <QIcon>
#include <QFontMetrics>

WorkTracker::WorkTracker(WorkTrackerController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkTracker)
    , m_controller(controller)
    , m_collapsedHeight(0)
{
    ui->setupUi(this);
    setupLanguageMenu();

    m_showAnimation.setTargetObject(this);
    m_showAnimation.setPropertyName("size");
    m_showAnimation.setDuration(200);

    m_hideAnimation.setTargetObject(this);
    m_hideAnimation.setPropertyName("size");
    m_hideAnimation.setDuration(200);

    ui->frame->setVisible(false);
    ui->textEdit->setVisible(false);

    m_statusRecording = new QLabel(this);
    m_statusDuration  = new QLabel(this);
    ui->statusBar->addWidget(m_statusRecording, 1);
    ui->statusBar->addWidget(m_statusDuration, 0);
    translate();

    // Capture the current width which is set by the designer. Then we resize the window
    // to only take up as much space as is needed with the edit field and text edit not
    // being visible. Since this also changes the width we restore it using the value
    // saved earliert. Voila, our UI looks correct.
    int width = this->width();
    this->adjustSize();
    this->resize(width, this->height());
    m_collapsedHeight = this->height();

    QCompleter *completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->tasksEdit->setCompleter(completer);

    connect(ui->workdayButton,    SIGNAL(clicked()),  m_controller, SLOT(toggleWorkDay()));
    connect(ui->selectTaskButton, SIGNAL(clicked()),  this,         SLOT(taskSelected()));
    connect(ui->taskButton,       SIGNAL(clicked()),  this,         SLOT(showInput()));
    connect(ui->summaryButton,    SIGNAL(clicked()),  this,         SLOT(showSummary()));
    connect(&m_showAnimation,     SIGNAL(finished()), this,         SLOT(showAnimationFinished()));

    connect(m_controller, SIGNAL(workDayStarted(QDateTime)),
            this,         SLOT(workDayStarted(QDateTime)));
    connect(m_controller, SIGNAL(workDayStopped(QDateTime)),
            this,         SLOT(workDayStopped(QDateTime)));
    connect(m_controller, SIGNAL(workTaskStarted(QDateTime, QString)),
            this,         SLOT(workTaskStarted(QDateTime, QString)));
    connect(m_controller, SIGNAL(workTaskStopped(QDateTime, QString)),
            this,         SLOT(workTaskStopped(QDateTime, QString)));
    connect(m_controller, SIGNAL(totalTimeChanged(int, int)),
            this,         SLOT(totalTimeChanged(int, int)));

    // Menu
    connect(ui->actionQuit,     SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionAbout_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAbout,    SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionEditor,   SIGNAL(triggered()), m_controller, SLOT(showEditor()));
    connect(ui->actionPreferences, &QAction::triggered, 
            m_controller,          &WorkTrackerController::showPreferences);
}

WorkTracker::~WorkTracker()
{
    delete ui;
}

void
WorkTracker::setTaskListModel(TaskListModel* model)
{
    ui->tasksEdit->completer()->setModel(model);
    m_taskModel = model;
}

void
WorkTracker::taskSelected()
{
    QString taskName = ui->tasksEdit->text();
    if (taskName.isEmpty()) {
        ui->statusBar->showMessage(tr("You must enter a task description"), 3000);
        return;
    }

    m_controller->toggleTask(taskName);

    ui->taskButton->setEnabled(true);
    ui->summaryButton->setEnabled(true);

    hideInput();
}

void
WorkTracker::workTaskStarted(QDateTime now, QString name)
{
    QString dateString = now.toLocalTime().toString(Qt::TextDate);

    hideSummary();
    ui->workdayButton->setEnabled(false);
    ui->taskButton->setEnabled(true);
    ui->taskButton->setText(tr("Stop &Task"));
    ui->taskButton->setIcon(QIcon(":/icon/Stop-Task.svg"));
    ui->tasksEdit->setText(name); // After restart this would be empty
    setShortenedTaskStatusText(tr("%1 started at %2").arg(name).arg(dateString));
}

void
WorkTracker::workTaskStopped(QDateTime now, QString name)
{
    QString dateString = now.toLocalTime().toString(Qt::TextDate);

    hideSummary();
    ui->workdayButton->setEnabled(true);
    ui->taskButton->setText(tr("Start &Task"));
    ui->taskButton->setIcon(QIcon(":/icon/Start-Task.svg"));

    setShortenedTaskStatusText(tr("%1 stopped at %2").arg(name).arg(dateString));
}

void
WorkTracker::showInput()
{
    ui->textEdit->setVisible(false);
    ui->taskButton->setEnabled(false);

    QSize size = this->size();
    m_showAnimation.setStartValue(size);

    size.setHeight(m_collapsedHeight + ui->frame->height());
    m_showAnimation.setEndValue(size);
    m_showAnimation.start();
    m_animatedWidget = ui->frame;
}

void
WorkTracker::hideInput()
{
    // Hide the frame and shrink the window
    ui->frame->setVisible(false);

    QSize size = this->size();
    m_hideAnimation.setStartValue(size);

    size.setHeight(m_collapsedHeight);
    m_hideAnimation.setEndValue(size);
    m_hideAnimation.start();
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
WorkTracker::workDayStarted(QDateTime now)
{
    hideSummary();

    ui->workdayButton->setText(tr("Stop Workday"));
    ui->workdayButton->setIcon(QIcon(":/icon/Stop-Day.svg"));
    ui->taskButton->setEnabled(true);
}

void
WorkTracker::workDayStopped(QDateTime now)
{
    ui->workdayButton->setText(tr("Start New Workday"));
    ui->workdayButton->setIcon(QIcon(":/icon/Start-Day.svg"));
    ui->taskButton->setEnabled(false);
    ui->summaryButton->setEnabled(!ui->textEdit->isVisible());
}

void
WorkTracker::showSummary()
{
    ui->summaryButton->setEnabled(false);

    if (!ui->textEdit->isVisible()) {
        ui->frame->setVisible(false);

        QString html = m_controller->generateSummary();
        ui->textEdit->setHtml(html);

        QSize size = this->size();
        m_showAnimation.setStartValue(size);

        size.setHeight(m_collapsedHeight + ui->textEdit->height());
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

        size.setHeight(m_collapsedHeight);
        m_hideAnimation.setEndValue(size);
        m_hideAnimation.start();
    }
}

void
WorkTracker::about()
{
    AboutDialog dlg;
    dlg.exec();
}

void
WorkTracker::setShortenedTaskStatusText(const QString& text) const
{
    int maxWidth = m_statusRecording->width();

    QFontMetrics metrics = m_statusRecording->fontMetrics();
    QString elidedString = metrics.elidedText(text, Qt::ElideMiddle, maxWidth);

    m_statusRecording->setText(elidedString);
}

void
WorkTracker::totalTimeChanged(int hours, int minutes)
{
    m_statusDuration->setText(tr("Total time %1h %2m").arg(hours).arg(minutes));
}

void 
WorkTracker::changeEvent(QEvent* p_event)
{
    if (QEvent::LanguageChange == p_event->type()) {
        ui->retranslateUi(this);
        translate();
    }
    
    QMainWindow::changeEvent(p_event);
}

void 
WorkTracker::translate()
{
    m_statusRecording->setText(tr("Not recording anything yet"));
    m_statusDuration->setText(tr("Total time %1h %2m").arg(0).arg(0));
}

void 
WorkTracker::setupLanguageMenu()
{
    ui->actionEnUS->setData(QVariant("en_US"));
    ui->actionDeDE->setData(QVariant("de_DE"));
    
    connect(ui->actionEnUS, &QAction::triggered, this, &WorkTracker::languageSelected);
    connect(ui->actionDeDE, &QAction::triggered, this, &WorkTracker::languageSelected);
    
    connect(this,         &WorkTracker::languageChanged, 
            m_controller, &WorkTrackerController::setLanguage);
}

void 
WorkTracker::languageSelected()
{
    QAction* source = qobject_cast<QAction*>(sender());
    if (nullptr == source) {
        qDebug() << "Sender is not a QAction. Cannot change language.";
        return;
    }
    
    QString locale = source->data().toString();
    setLanguageChecked(locale);
    emit languageChanged(locale);
}

void 
WorkTracker::setLanguageChecked(const QString& p_locale)
{
    for (QAction* action : ui->menuLanguage->actions()) {
        QString locale = action->data().toString();
        action->setChecked(locale == p_locale);
    }
}
