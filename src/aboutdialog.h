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

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
