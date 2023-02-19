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
    
protected:
    /*!
     * Acts on the `QEvent::LanguageChange` event and retranslates the ui.
     * 
     * \param p_event
     * The event that happened.
     */
    void changeEvent(QEvent* p_event);

private:
    Ui::SelectWorkdayWidget* ui;
    EditorController*        m_controller;
    bool                     m_isComplete;

};

#endif // SELECTWORKDAYPAGE_H
