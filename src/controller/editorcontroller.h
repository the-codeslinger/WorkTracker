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

#include <QObject>

class QDomDocument;
class WorkDayModel;

/*!
 * Handles all the requests and necessary tasks that are part of editing workdays and
 * their worktask items.
 */
class EditorController : public QObject
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the controller.
     *
     * \param p_dataSource
     * The loaded XML database. The datasource itself is owned by calling code, the
     * controller only stores a reference for use.
     *
     * \param p_parent
     * Parent of the controller.
     */
    explicit EditorController(QDomDocument* p_dataSource, QObject* p_parent = nullptr);

    /*!
     * \return
     * Returns a list-model for use with workdays.
     */
    WorkDayModel* workDayModel(QObject* p_parent) const;

signals:

public slots:

private:
    /*!
     * The loaded XML database.
     */
    QDomDocument* m_dataSource;

};

#endif // EDITORCONTROLLER_H
