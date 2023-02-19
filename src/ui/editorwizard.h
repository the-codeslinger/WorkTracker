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
