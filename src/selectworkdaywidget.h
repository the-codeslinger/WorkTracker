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

#ifndef SELECTWORKDAYWIDGET_H
#define SELECTWORKDAYWIDGET_H

#include <QWidget>

namespace Ui {
    class SelectWorkdayWidget;
}

class EditorController;
class QItemSelectionModel;

/*!
 * Widget component used by `SelectWorkdayPage` to show a list view of available workdays
 * that can be edited.
 */
class SelectWorkdayWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the widget.
     *
     * \param p_controller
     * The controller that will handle all the business logic for this widget.
     *
     * \param p_parent
     * Parent of the widget.
     */
    explicit SelectWorkdayWidget(EditorController* p_controller,
                                 QWidget* p_parent = nullptr);

    /*!
     * Releases the resources of the UI widgets.
     */
    ~SelectWorkdayWidget();

    /**
     * @return
     * Returns the selection model of the list view. Can be used to track selection
     * changes.
     */
    QItemSelectionModel* selectionModel() const;

private:
    Ui::SelectWorkdayWidget *ui;
};

#endif // SELECTWORKDAYWIDGET_H
