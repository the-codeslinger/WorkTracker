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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

/*!
 * Simple about dialog with the version number and some links to related websites.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Creates a new WorkTracker about dialog.
     *
     * \param parent
     * Parent of the dialog.
     */
    explicit AboutDialog(QWidget* parent = nullptr);

    /*!
     * Deletes the ui resources.
     */
    ~AboutDialog();
    
protected:
    /*!
     * Acts on the `QEvent::LanguageChange` event and retranslates the ui.
     * 
     * \param p_event
     * The event that happened.
     */
    void changeEvent(QEvent* p_event);

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
