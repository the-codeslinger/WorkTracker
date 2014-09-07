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

/*!
 * \brief Dialog based user interface implementation.
 *
 * Sets up the UI according to the forms file and contains slots that match the
 * controller's signals.
 */
class WorkTracker : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * Creates a new WorkTracker main window.
     *
     * \param controller
     * The controller to use to process the user's interaction with the UI.
     *
     * \param parent
     * Parent of the widget.
     */
    explicit WorkTracker(WorkTrackerController* controller, QWidget *parent = 0);

    /*!
     * Releases the resources of the UI widgets.
     */
    ~WorkTracker();

    /*!
     * Allows to set a `model` that adheres to the Qt Model View architecture. This is
     * used to show suggestions to the user based on previous input.
     */
    void setTaskListModel(TaskListModel* model);
    
    /**
     * Set a specific language menu item to checked. This unchecks all other items.
     * 
     * @param p_locale
     * The locale of the item to check. `QAction::setData(const QVariant&)` must have been
     * called on the menu actions with the locale for this to work.
     */
    void setLanguageChecked(const QString& p_locale);
    
signals:
    /*!
     * Emitted when a different language has been selected in the ui.
     *
     * \param p_locale
     * The locale of the selected language.
     */
    void languageChanged(const QString& p_locale);

public slots:
    /*!
     * \see
     * WorkTrackerController::workDayStarted(QDateTime)
     */
    void workDayStarted(QDateTime now);

    /*!
     * \see
     * WorkTrackerController::workDayStopped(QDateTime)
     */
    void workDayStopped(QDateTime now);

    /*!
     * \see
     * WorkTrackerController::workTaskStarted(QDateTime, QString)
     */
    void workTaskStarted(QDateTime now, QString name);

    /*!
     * \see
     * WorkTrackerController::workTaskStopped(QDateTime, QString)
     */
    void workTaskStopped(QDateTime now, QString name);

    /*!
     * \see
     * WorkTrackerController::totalTimeChanged(int, int)
     */
    void totalTimeChanged(int hours, int minutes);

private slots:
    /*!
     * The user clicked on the "Select Task" button to confirm the name of a task.
     */
    void taskSelected();

    /*!
     * Called at the end of the show-animation to set visible on the to-be-shown UI
     * widget.
     */
    void showAnimationFinished();

    /*!
     * The user clicked on the "Show Summary" button.
     */
    void showSummary();

    /*!
     * Called whenever a new day has been started or as task has been stopped or started.
     * Hides the summary widget.
     */
    void hideSummary();

    /*!
     * The user clicked the "Start / Stop Task" button. Expands the UI to display the
     * input field for the name of the task.
     */
    void showInput();

    /*!
     * Called when the user confirms the name of a task. Hides the input field.
     */
    void hideInput();

    /*!
     * Shows the about dialog
     */
    void about();
    
    /*!
     * Handles the `QAction::triggered` signals related to language selection;
     */
    void languageSelected();
    
protected:
    /*!
     * Acts on the `QEvent::LanguageChange` event and retranslates the ui.
     * 
     * \param p_event
     * The event that happened.
     */
    void changeEvent(QEvent* p_event);

private:
    /*!
     * Contains the UI widgets from the form file.
     */
    Ui::WorkTracker* ui;
    /*!
     * The model used for suggestions.
     */
    TaskListModel* m_taskModel;
    /*!
     * Label for the left part of the status bar.
     */
    QLabel* m_statusDuration;
    /*!
     * Label for the right part of the status bar.
     */
    QLabel* m_statusRecording;

    /*!
     * Temporary storage so the `showAnimationFinished()` slot knows which widget to set
     * visible.
     */
    QWidget* m_animatedWidget;

    /*!
     * Animator to expand the window.
     */
    QPropertyAnimation m_showAnimation;
    /*!
     * Animator to shrink the window.
     */
    QPropertyAnimation m_hideAnimation;

    /*!
     * The controller, obviously.
     */
    WorkTrackerController* m_controller;

    /*!
     * Recorded in the constructor. Contains the minimum size of the collapsed ui which
     * is always used to shrink back to the small size.
     */
    int m_collapsedHeight;

    /**
     * Uses the task status label's font metrics to calculate a shortened version of the
     * text in order to not resize the window.
     *
     * @param text
     * The full text that shall be displayed and eventually shortened.
     */
    void setShortenedTaskStatusText(const QString& text) const;
    
    /*!
     * Sets the user visible strings on widgets not created by designer, e.g. status bar
     * labels.
     */
    void translate();
    
    /*!
     * Sets custom data for all language based `QAction` objects. Every action then 
     * contains the locale-string which is easy to query by event handlers.
     */
    void setupLanguageMenu();
};

#endif // WORKTRACKER_H
