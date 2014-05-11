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

#ifndef TASK_H
#define TASK_H

#include "xmldata.h"

#include <QString>
#include <QDate>
#include <QDomNode>
#include <QList>
#include <QVariant>

#include <functional>

class QDomDocument;
class QDomAttr;

class Task : public XmlData
{
public:
    static int invalidId;

    Task();
    Task(QDomDocument* dataSource);
    Task(QDomDocument* dataSource, QDomElement node);
    Task(QDomDocument* dataSource, QString name, QDate lastUsed);
    Task(QDomDocument* dataSource, int id, QString name, QDate lastUsed);
    Task(const Task& other);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QDate lastUsed() const;
    void setLastUsed(const QDate& lastUsed);

    // Used to fetch from the data source
    static Task fromDomNode(QDomElement node, QDomDocument* dataSource);
    static Task get(int id, QDomDocument* dataSource);
    static Task findByName(QString name, QDomDocument* dataSource);
    static QList<Task> list(QDomDocument* dataSource);
    static int count(QDomDocument* dataSource);

private:

    void createNode(int id, QString name, QDate lastUsed);
    QVariant attributeValue(QString name) const;

    static Task findTask(QDomDocument* dataSource, std::function<bool(Task)> predicate);

    static QDomNodeList getTaskNodes(QDomDocument* dataSource);
};

#endif // TASK_H
