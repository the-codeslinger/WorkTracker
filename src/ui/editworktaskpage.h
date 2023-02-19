/*
 * Copyright 2023 Robert Lohr
 *
 * This file is part of WorkTracker.
 *
 * WorkTracker is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * WorkTracker is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with
 * WorkTracker. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef EDITWORKTASKPAGE_H
#define EDITWORKTASKPAGE_H

#include <QWizardPage>
#include <QModelIndexList>

namespace Ui {
    class EditWorktaskWidget;
}

class EditorController;
class QItemSelection;
class QListView;
class QTableView;
class SelectWorkDayPage;

/*!
 * UI page to be used with a `QWizard` that shows the list to edit worktask items.
 */
class EditWorkTaskPage : public QWizardPage
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the wizard page.
     *
     * \param controller
     * The controller that will handle all the business logic for this widget.
     * 
     * \param workDayPage
     * Reference to the previous page. Used to fetch the selected item.
     *
     * \param parent
     * Parent of the widget.
     */
    EditWorkTaskPage(EditorController* controller, SelectWorkDayPage* workDayPage,
                     QWidget* parent = nullptr);

    /*!
     * Releases the resources of the UI widgets.
     */
    ~EditWorkTaskPage();

    /*!
     * initialize the page with the selected work day from the previous page.
     */
    void initializePage();

    /*!
     * \return
     * Returns the indexes of the selected tasks.
     */
    QModelIndexList selectedTasks() const;
    
    /*!
     * \return
     * Returns the index of the selected time-row.
     */
    QModelIndexList selectedTimes() const;

public slots:
    /*!
     * Handles a selected work-ttask item. This refreshes the table view to contain the 
     * values of the newly selected item.
     */
    void taskSelected(const QItemSelection& selection);
    
    /*!
     * Handles a selected task-time item. This enables or disables the delete button for
     * the time table.
     */
    void timeSelected(const QItemSelection& selection);
    
protected slots:
    /*!
     * This slot is connected to `SelectedWorkDayModel::taskAlreadyExists(const QString&)`
     * and shows a message box when triggered.
     */
    void taskAlreadyExists(const QString& name);
    
    /*!
     * Sets the error message label to `error`.
     */
    void validationError(const QString& error);
    
    /*!
     * Resets the error message to nothing
     */
    void validationSuccess();

    /*!
     * Shows a dialog box for task input and then emits 
     * `EditWorkTaskPage::addTask(const QString&)` with the name of the task.
     */
    void addTask();
    
    /*!
     * Removes all the selected tasks from the model.
     */
    void removeTask();
    
    /*!
     * Adds another row to the model of times. The start and stop values are null.
     */
    void addTime();
    
    /*!
     * Removes all the selected times from the model.
     */
    void removeTime();

    /*!
     * Calls the controller's validation method.
     */
    void validateModel();
    
protected:
    /*!
     * Acts on the `QEvent::LanguageChange` event and retranslates the ui.
     * 
     * \param event
     * The event that happened.
     */
    void changeEvent(QEvent* event);

private:
    Ui::EditWorktaskWidget* ui;
    EditorController*       m_controller;
    SelectWorkDayPage*      m_workDayPage;

};

#endif // EDITWORKTASKPAGE_H
