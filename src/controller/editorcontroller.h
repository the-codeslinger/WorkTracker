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

#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include "abstractcontroller.h"
#include "../model/datasource.h"

#include <QObject>

class WorkTask;
class WorkDay;

/*!
 * Handles all the requests and necessary tasks that are part of editing workdays and
 * their worktask items.
 */
class EditorController : public QObject, public AbstractController
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the controller.
     *
     * \param dataSource
     * The loaded XML database. The datasource itself is owned by calling code, the
     * controller only stores a reference for use.
     *
     * \param parent
     * Parent of the controller.
     */
    EditorController(DataSource dataSource, QObject* parent = nullptr);
    
signals:
    /*!
     * Emitted by `EditorController::validateModel()` if an error has been found in the
     * currently edited work-day's model.
     * 
     * \param error
     * A description of the error.
     */
    void validationError(const QString& error);
    
    /*!
     * Emitted by `EditorController::validateModel()` if the currently edited work-day's 
     * model passed validation without an error.
     */
    void validationSuccess();
    
    /*!
     * Emitted when the wizard is closed (no matter how) and there's an active task.
     */
    void activeTaskChanged(const WorkTask& task);
    
    /*!
     * Emitted when the wizard is closed (no matter how) and there's no more active task.
     */
    void currentTaskClosed();

public slots:
    /*!
     * Tests the currently used workday's contents if they are valid. Emits 
     * `EditorController::contentError(const QString&)` in case of an error or
     * `EditorController::validationSuccess()` if no error was found.
     */
    void validateModel(const WorkDay& workDay);
    
    /*!
     * Evaluate the current work-day and update the `WorkTrackerController` with the 
     * currently active work-task and work-time based on the changes.
     */
    void updateActiveWorkTasks();

};

#endif // EDITORCONTROLLER_H
