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

#include "xmldata.h"

#include <QString>
#include <QDate>
#include <QDateTime>

XmlData::XmlData()
{ }

XmlData::XmlData(const QDomDocument& dataSource, const QString& name)
    : m_dataSource(dataSource)
{
    if (!dataSource.isNull()) {

        m_element = m_dataSource.createElement(name);
    }
}

XmlData::XmlData(const QDomDocument& dataSource, const QDomElement& element)
    : m_dataSource(dataSource)
    , m_element(element)
{ }

XmlData::XmlData(const XmlData& other)
    : m_dataSource(other.m_dataSource)
    , m_element(other.m_element)
{ }

XmlData&
XmlData::operator=(const XmlData& other)
{
    m_dataSource = other.m_dataSource;
    m_element = other.m_element;
    return *this;
}

QDomDocument
XmlData::dataSource() const
{
    return m_dataSource;
}

void
XmlData::setDataSource(const QDomDocument& dataSource)
{
    // New data source means to reset the connection to the current DOM.
    m_element.clear();
    m_dataSource = dataSource;
    if (!dataSource.isNull()) {
        m_element = m_dataSource.createElement(elementName());
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
    QString string;
    if (!value.isNull()) {
        string = value.toUTC().toString(Qt::ISODate);
    }
    
    setAttribute(name, string);
}

void 
XmlData::setAttribute(const QString& name, const QDate& value)
{
    QString string;
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
    QString dt = attributeString(name, element);
    bool ok = false;
    int number = dt.toInt(&ok);
    return ok ? number : XmlData::invalidId;
}
