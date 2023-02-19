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

#include "xmldata.h"

#include <QString>
#include <QDate>
#include <QDateTime>

XmlData::XmlData()
{ }

XmlData::XmlData(DataSource dataSource, const QString& name)
    : m_dataSource{std::move(dataSource)}
{
    if (!dataSource.isNull()) {
        m_element = m_dataSource.document().createElement(name);
    }
}

XmlData::XmlData(DataSource dataSource, const QDomElement& element)
    : m_dataSource{std::move(dataSource)}
    , m_element{element}
{ }

XmlData::XmlData(const XmlData& other)
    : m_dataSource{other.m_dataSource}
    , m_element{other.m_element}
{ }

XmlData&
XmlData::operator=(const XmlData& other)
{
    m_dataSource = other.m_dataSource;
    m_element    = other.m_element;
    return *this;
}

DataSource
XmlData::dataSource() const
{
    return m_dataSource;
}

void
XmlData::setDataSource(DataSource dataSource)
{
    // New data source means to reset the connection to the current DOM.
    m_element.clear();
    m_dataSource = std::move(dataSource);
    if (!dataSource.isNull()) {
        m_element = m_dataSource.document().createElement(elementName());
    }
}

QDomElement
XmlData::element() const
{
    return m_element;
}

void
XmlData::setElement(const QDomElement& element)
{
    m_element = element;
}

QDomNode
XmlData::parent() const
{
    if (m_element.isNull()) {
        return QDomNode();
    }

    return m_element.parentNode();
}

void
XmlData::appendChild(const XmlData& child)
{
    if (!m_element.isNull() && !child.isNull()) {
        m_element.appendChild(child.element());
    }
}

bool
XmlData::isNull() const
{
    return m_element.isNull();
}

void
XmlData::clear()
{
    if (!m_element.isNull()) {
        auto parent = m_element.parentNode();
        if (!parent.isNull()) {
            parent.removeChild(m_element);
        }

        m_element.clear();
    }
}

bool 
XmlData::operator==(const XmlData& other) const
{
    return m_element == other.m_element;
}

bool 
XmlData::operator!=(const XmlData& other) const
{
    return m_element != other.m_element;
}

void
XmlData::setAttribute(const QString& name, const QString& value)
{
    if (m_element.isNull()) {
        return;
    }

    m_element.setAttribute(name, value);
}

void 
XmlData::setAttribute(const QString& name, const QDateTime& value)
{
    auto string = QString{};
    if (!value.isNull()) {
        string = value.toUTC().toString(Qt::ISODate);
    }
    
    setAttribute(name, string);
}

void 
XmlData::setAttribute(const QString& name, const QDate& value)
{
    auto string = QString{};
    if (!value.isNull()) {
        string = value.toString(Qt::ISODate);
    }
    
    setAttribute(name, string);
}

void 
XmlData::setAttribute(const QString& name, int value)
{
    setAttribute(name, QString::number(value));
}

QString 
XmlData::attributeString(const QString& name) const
{
    return attributeString(name, m_element);
}

QString 
XmlData::attributeString(const QString& name, const QDomElement& element) const
{
    return element.attribute(name);
}

QDateTime 
XmlData::attributeDateTime(const QString& name) const
{
    return attributeDateTime(name, m_element);
}

QDateTime 
XmlData::attributeDateTime(const QString& name, const QDomElement& element) const
{
    return QDateTime::fromString(attributeString(name, element), Qt::ISODate);
}

QDate
XmlData::attributeDate(const QString& name) const
{
    return attributeDate(name, m_element);
}

QDate 
XmlData::attributeDate(const QString& name, const QDomElement& element) const
{
    return QDate::fromString(attributeString(name, element), Qt::ISODate);
}

int 
XmlData::attributeInt(const QString& name) const
{
    return attributeInt(name, m_element);
}

int 
XmlData::attributeInt(const QString& name, const QDomElement& element) const
{
    auto dt = attributeString(name, element);
    auto ok = false;
    auto number = dt.toInt(&ok);
    return ok ? number : XmlData::invalidId;
}
