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

#ifndef SELECTWORKDAYPAGE_H
#define SELECTWORKDAYPAGE_H

#include <QWizardPage>

namespace Ui {
    class SelectWorkdayWidget;
}

class EditorController;
class QItemSelection;

/*!
 * UI page to be used with a `QWizard` that shows the list to select the workday that
 * shall be edited.
 */
class SelectWorkDayPage : public QWizardPage
{
    Q_OBJECT
public:
    /*!
     * Create a new wizard page to select a workday.
     *
     * \param p_parent
     * Parent of the page.
     */
    explicit SelectWorkDayPage(EditorController* p_controller,
                               QWidget* p_parent = nullptr);

    /*!
     * Releases the resources of the UI widgets.
     */
    ~SelectWorkDayPage();

    /*!
     * \return
     * Only returns `true` if one item in the list has been selected.
     */
    bool isComplete() const;

    /*!
     * Selects the first item if there is any.
     */
    void initializePage();

    /*!
     * \return
     * Returns the selected item.
     */
    QVariant selectedItem() const;

public slots:
    /**
     * Sets `m_isComplete` to true if `p_index` is valid.
     */
    void itemSelected(const QModelIndex& p_index);

private:
    Ui::SelectWorkdayWidget* ui;
    EditorController*        m_controller;
    bool                     m_isComplete;

};

#endif // SELECTWORKDAYPAGE_H
