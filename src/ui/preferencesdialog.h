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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class PreferencesDialog;
}

class PreferencesController;

/*!
 * Does as the name implies. Contains the ui and the ui-logic of a dialog to select
 * different settings of the application. Business logic is encapsulated in 
 * `SettingsController` which creates this dialog.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Create a new dialog instance.
     * 
     * \param p_controller
     * The controller that manages this dialog.
     * 
     * \param p_parent
     * The dialog's parent widget.
     */
    PreferencesDialog(PreferencesController* p_controller, QWidget* p_parent = nullptr);
    
    /*!
     * Releases the ui resources.
     */
    ~PreferencesDialog();
    
protected:
    /*!
     * Acts on the `QEvent::LanguageChange` event and retranslates the ui.
     *
     * \param p_event
     * The event that happened.
     */
    void changeEvent(QEvent* p_event);
    
private:
    Ui::PreferencesDialog* ui;
    PreferencesController* m_controller;
};

#endif // SETTINGSDIALOG_H
