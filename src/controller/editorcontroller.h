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

#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <QObject>
#include <QDomDocument>

class WorkDayModel;
class WorkTaskModel;
class SelectedWorkDayModel;
class SelectWorkDayPage;
class EditWorkTaskPage;

/*!
 * Handles all the requests and necessary tasks that are part of editing workdays and
 * their worktask items.
 */
class EditorController : public QObject
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the controller.
     *
     * \param p_dataSource
     * The loaded XML database. The datasource itself is owned by calling code, the
     * controller only stores a reference for use.
     *
     * \param p_parent
     * Parent of the controller.
     */
    EditorController(const QDomDocument& p_dataSource, QObject* p_parent = nullptr);

    /**
     * Show a modal wizard dialog and block until it is finished.
     */
    void run();

    /*!
     * Set the wizard page that lets the user select a workday for editing.
     */
    void setWizardPage(SelectWorkDayPage* p_page);

    /*!
     * Set the wizard page that lets the
     */
    void setWizardPage(EditWorkTaskPage* p_page);

    /*!
     * Sets context specific data for the model of the select-workday page.
     */
    void setModelData(WorkDayModel* p_model);

    /*!
     * Sets context specific data for the select-task model of the edit-workday page.
     */
    void setModelData(SelectedWorkDayModel* p_model);

    /*!
     * Sets context specific data for the edit-worktask model of the edit-workday page.
     */
    void setModelData(const QModelIndex& p_index, SelectedWorkDayModel* p_source,
                      WorkTaskModel* p_destination);
    
    /*!
     * \return 
     * Returns the data-source to use for models.
     */
    QDomDocument dataSource() const;

signals:

public slots:
    /*!
     *
     */
    void addTask();
    
    /*!
     *
     */
    void removeTask();
    
    /*!
     *
     */
    void addTime();
    
    /*!
     *
     */
    void removeTime();

private:
    /*!
     * The loaded XML database.
     */
    QDomDocument m_dataSource;

    SelectWorkDayPage* m_selectWorkDayPage;
    EditWorkTaskPage*  m_editWorkTaskPage;

};

#endif // EDITORCONTROLLER_H
