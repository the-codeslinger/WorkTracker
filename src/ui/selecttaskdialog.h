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

#ifndef SELECTTASKDIALOG_H
#define SELECTTASKDIALOG_H

#include "../model/datasource.h"

#include <QDialog>
#include <QDomDocument>

namespace Ui {
    class SelectTaskDialog;
}

/*!
 * A simple dialog with just one line-edit in which the user can search for existing 
 * tasks or enter a new one.
 */
class SelectTaskDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Create a new dialog. The data-source is needed to create the model that the 
     * completer uses.
     */
    SelectTaskDialog(DataSource dataSource, QWidget* parent = nullptr);
    
    /*!
     * Releases the ui resources.
     */
    ~SelectTaskDialog();
    
    /*!
     * \return 
     * Returns the name of the task the user selected or entered.
     */
    QString taskName() const;
    
protected slots:
    /*!
     * Evaluates the current content of the line-edit and enables or disables the OK
     * button accordingly.
     */
    void textChanged(const QString& text);
    
private:
    Ui::SelectTaskDialog* ui;
};

#endif // SELECTTASKDIALOG_H
