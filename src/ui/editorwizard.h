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

#ifndef EDITORWIZARD_H
#define EDITORWIZARD_H

#include <QWizard>

class EditorController;
class SelectWorkDayPage;
class EditWorkTaskPage;

/*!
 * Wizard style editor to edit the contents of the database.
 */
class EditorWizard : public QWizard
{
    Q_OBJECT
public:
    /*!
     * Create a new editor dialog.
     * 
     * \param controller
     * The controller that contains the business logic.
     * 
     * \param parent
     * The parent widget of the editor dialog.
     */
    EditorWizard(EditorController* controller, QWidget* parent = nullptr);
    
public slots:
    /*!
     * Disables the "back" and "finish" button.
     */
    void validationError();
    
    /*!
     * Eables the "back" and "finish" button.
     */
    void validationSuccess();
    
protected:
    /*!
     * Resizes the window by 1.10.
     */
    void showEvent(QShowEvent* event);

private:
    EditorController*  m_controller;
    SelectWorkDayPage* m_selectWorkDayPage;
    EditWorkTaskPage*  m_editWorkTaskPage;
    bool               m_resized;
    
};

#endif // EDITORWIZARD_H
