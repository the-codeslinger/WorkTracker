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

#ifndef WORKTASK_H
#define WORKTASK_H

#include "task.h"
#include "xmldata.h"

#include <QList>
#include <QDateTime>
#include <QPair>

class QDomNode;
class QDomDocument;

class WorkTask : public XmlData
{
public:
    WorkTask();
    WorkTask(QDomDocument* dataSource);
    WorkTask(QDomDocument* dataSource, Task task, QDateTime start, QDateTime stop);

    Task task() const;
    void setTask(Task task);

    QDateTime start() const;
    void setStart(QDateTime start);

    QDateTime stop() const;
    void setStop(QDateTime stop);

    int totalTime() const;
    bool isNull() const;
    void clear();

    static QList<WorkTask> fromDomNode(QDomNode* node, QDomDocument* dataSource);

private:
    Task      m_task;
    QDateTime m_start;
    QDateTime m_stop;

    static int idFromAttr(QDomNode* attr);
    static QDateTime timestampFromAttr(QDomNode* attr);
};

#endif // WORKTASK_H
