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

#include "task.h"

#include <QDate>
#include <QString>

#include <algorithm>

static const QString g_elementName = "item";

Task::Task()
    : XmlData{}
{ }

Task::Task(DataSource dataSource, const QString& name, const QDateTime& lastUsed)
    : Task{std::move(dataSource), Task::invalidId, name, lastUsed}
{ }

Task::Task(DataSource dataSource, int p_id, const QString& name, const QDateTime& lastUsed)
    : XmlData{std::move(dataSource), g_elementName}
{
    setAttribute("id", p_id);
    setAttribute("name", name);
    setAttribute("last_used", lastUsed);
}

Task::Task(DataSource dataSource, const QDomElement& node)
    : XmlData{std::move(dataSource), node}
{ }

Task::Task(const Task& other)
    : XmlData{other}
{ }

int
Task::id() const
{
    return attributeInt("id");
}

void
Task::setId(int p_id)
{
    setAttribute("id", p_id);
}

QString
Task::name() const
{
    return attributeString("name");
}

void
Task::setName(const QString& name)
{
    setAttribute("name", name);
}

QDateTime
Task::lastUsed() const
{
    return attributeDateTime("last_used");
}

void
Task::setLastUsed(const QDateTime& lastUsed)
{
    setAttribute("last_used", lastUsed);
}

QString 
Task::elementName() const
{
    return g_elementName;
}
