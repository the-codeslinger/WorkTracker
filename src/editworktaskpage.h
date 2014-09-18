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
     * \param p_controller
     * The controller that will handle all the business logic for this widget.
     *
     * \param p_parent
     * Parent of the widget.
     */
    EditWorkTaskPage(EditorController* p_controller, QWidget* p_parent = nullptr);

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
    
    /*!
     * \return 
     * Returns the list-view of the work-tasks.
     */
    QListView* workTasksView() const;
    
    /*!
     * \return 
     * Returns the list-view of the work-times.
     */
    QTableView* workTimesView() const;
    
    /*!
     * Evaluates the changes and shows a message-box that contains all the errors that
     * prevent continuing.
     * 
     * \return 
     */
    bool validatePage();

public slots:
    /*!
     * Handles a selected work-ttask item. This refreshes the table view to contain the 
     * values of the newly selected item.
     */
    void taskSelected(const QItemSelection& p_selection);
    
    /*!
     * Handles a selected task-time item. This enables or disables the delete button for
     * the time table.
     */
    void timeSelected(const QItemSelection& p_selection);
    
protected slots:
    /*!
     * This slot is connected to `SelectedWorkDayModel::taskAlreadyExists(const QString&)`
     * and shows a message box when triggered.
     */
    void taskAlreadyExists(const QString& p_name);
    
    void validationError(const QString& p_error);
    
protected:
    /*!
     * Acts on the `QEvent::LanguageChange` event and retranslates the ui.
     * 
     * \param p_event
     * The event that happened.
     */
    void changeEvent(QEvent* p_event);

private:
    Ui::EditWorktaskWidget* ui;
    EditorController*       m_controller;

};

#endif // EDITWORKTASKPAGE_H
