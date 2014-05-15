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

#ifndef WORKDAY_H
#define WORKDAY_H

#include "worktask.h"
#include "xmldata.h"

#include <QDateTime>
#include <QList>
#include <QDomElement>

class QDomDocument;

class WorkDay : public XmlData
{
public:
    WorkDay();
    WorkDay(QDomDocument* dataSource);
    WorkDay(QDomDocument* dataSource, QDomElement node);
    WorkDay(QDomDocument* dataSource, QDateTime start);
    WorkDay(QDomDocument* dataSource, QDateTime start, QDateTime stop);

    QDateTime start() const;
    void setStart(QDateTime start);

    QDateTime stop() const;
    void setStop(QDateTime stop);

    void addTask(WorkTask task);
    void clear();

    QString generateSummary() const;


    static WorkDay findLastOpen(QDomDocument* dataSource);

private:
    QList<WorkTask> m_tasks;

    void createNode(QDateTime start, QDateTime stop);

    QDomElement findTask(int id) const;
    float roundTwoDecimals(float number) const;

    static WorkDay fromDomNode(QDomElement node, QDomDocument* dataSource);
};

#endif // WORKDAY_H
