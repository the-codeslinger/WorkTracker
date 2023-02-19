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
