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

#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>

class PreferencesDialog;
class QString;

/*!
 * Manages the actions of the `SettingsDialog`.
 */
class PreferencesController : public QObject
{
    Q_OBJECT
public:
    /*!
     * Creates a new settings controller and dialog. The ui is shown by calling
     * `SettingsController::exec()`.
     * 
     * \param p_parentWidget
     * The parent widget of the dialog.
     * 
     * \param p_parent
     * The controller's parent.
     */
    PreferencesController(QWidget* p_parentWidget, QObject* p_parent = nullptr);
    
    /*!
     * Shows a modal settings dialog but does not block.
     */
    void run();
    
signals:
    /*!
     * Emitted when the dialog is closed.
     * 
     * \param p_saved
     * `true` if the settings dialog was closed due to a click on the "Save" button or
     * `false` if all changes have been discarded.
     */
    void finished(bool p_saved);

    /*!
     * Emitted when a different language has been selected in the ui.
     *
     * \param p_locale
     * The locale of the selected language.
     */
    void languageChanged(const QString& p_locale);
    
public slots:
    /*!
     * Saves the settings.
     */
    void saveSettings();
    
    /*!
     * Restore the settings to the state before opening the dialog. This only applies to
     * settings that take immediate effect, e.g. changing the language.
     */
    void restoreSettings();
    
private:
    PreferencesDialog* m_dialog;
    
};

#endif // SETTINGSCONTROLLER_H
