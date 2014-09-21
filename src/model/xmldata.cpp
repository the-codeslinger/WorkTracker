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

#include <QDebug>
#include <QVariant>
#include <QDomDocument>
#include <QDateTime>

XmlData::XmlData()
{
}

XmlData::XmlData(const QDomDocument& p_dataSource)
    : m_dataSource(p_dataSource)
{
}

XmlData::XmlData(const QDomDocument& p_dataSource, const QDomElement& p_node)
    : m_dataSource(p_dataSource)
    , m_node(p_node)
{
}

XmlData::XmlData(const QDomDocument& p_dataSource, const QDomElement& p_node, 
                 const QDomNode& p_parent)
    : m_dataSource(p_dataSource)
    , m_node(p_node)
    , m_parent(p_parent)
{
}

XmlData::XmlData(const XmlData& p_other)
    : m_dataSource(p_other.m_dataSource)
    , m_node(p_other.m_node)
    , m_parent(p_other.m_parent)
{
}

QDomDocument
XmlData::dataSource() const
{
    return m_dataSource;
}

void
XmlData::setDataSource(const QDomDocument& p_dataSource)
{
    m_dataSource = p_dataSource;
}

QDomElement
XmlData::node() const
{
    return m_node;
}

void
XmlData::setNode(const QDomElement& p_node)
{
    m_node = p_node;
    setParent(m_parent);
}

QDomNode
XmlData::parent() const
{
    return m_parent;
}

void
XmlData::setParent(const QDomNode& p_parent)
{
    if (p_parent.isNull()) {
        return;
    }
    
    m_parent = p_parent;
    if (!m_node.isNull()) {
        m_parent.appendChild(m_node);
    }
}

void 
XmlData::setParent(const XmlData& p_parent)
{
    setParent(p_parent.parent());
}

bool
XmlData::isNull() const
{
    return m_node.isNull();
}

void
XmlData::clear()
{
    m_node.clear();
    m_parent.clear();
}

bool 
XmlData::operator==(const XmlData& p_other) const
{
    return m_node == p_other.m_node;
}

bool 
XmlData::operator!=(const XmlData& p_other) const
{
    return m_node != p_other.m_node;
}

void
XmlData::remove()
{
    if (!m_node.isNull()) {
        if (!m_parent.isNull()) {
            m_parent.removeChild(m_node);
        }
        
        m_node.clear();
    }
}

XmlData&
XmlData::operator=(const XmlData& p_other)
{
    m_dataSource = p_other.m_dataSource;
    m_node       = p_other.m_node;
    m_parent     = p_other.m_parent;
    return *this;
}

void
XmlData::setAttribute(const QString& p_name, const QString& p_value)
{
    if (m_node.isNull()) {
        qDebug() << "node is null";
        return;
    }

    QDomAttr attr = attribute(p_name);
    if (attr.isNull()) {
        attr = m_dataSource.createAttribute(p_name);
        m_node.setAttributeNode(attr);
    }
    attr.setValue(p_value);
}

void 
XmlData::setAttribute(const QString& p_name, const QDateTime& p_value)
{
    QString value;
    if (!p_value.isNull()) {
        value = p_value.toUTC().toString(Qt::ISODate);
    }
    
    setAttribute(p_name, value);
}

void 
XmlData::setAttribute(const QString& p_name, const QDate& p_value)
{
    QString value;
    if (!p_value.isNull()) {
        value = p_value.toString(Qt::ISODate);
    }
    
    setAttribute(p_name, value);
}

void 
XmlData::setAttribute(const QString& p_name, int p_value)
{
    if (-1 == p_value) {
        return;
    }
    
    setAttribute(p_name, QString::number(p_value));
}

QString 
XmlData::attributeString(const QString& p_name) const
{
    return attributeString(p_name, m_node);
}

QString 
XmlData::attributeString(const QString& p_name, const QDomNode& p_node) const
{
    return attribute(p_name, p_node).value();
}

QDateTime 
XmlData::attributeDateTime(const QString& p_name) const
{
    return attributeDateTime(p_name, m_node);
}

QDateTime 
XmlData::attributeDateTime(const QString& p_name, const QDomNode& p_node) const
{
    QString dt = attribute(p_name, p_node).value();
    if (dt.isNull()) {
        return QDateTime();
    }
    
    return QDateTime::fromString(dt, Qt::ISODate);
}

QDate
XmlData::attributeDate(const QString& p_name) const
{
    return attributeDate(p_name, m_node);
}

QDate 
XmlData::attributeDate(const QString& p_name, const QDomNode& p_node) const
{
    QString dt = attribute(p_name, p_node).value();
    if (dt.isNull()) {
        return QDate();
    }
    
    return QDate::fromString(dt, Qt::ISODate);
}

int 
XmlData::attributeInt(const QString& p_name) const
{
    return attributeInt(p_name, m_node);
}

int 
XmlData::attributeInt(const QString& p_name, const QDomNode& p_node) const
{
    QString dt = attribute(p_name, p_node).value();
    if (dt.isEmpty()) {
        // Only positive numbers are needed by the application. Therefore -1 can be used
        // as an invalid number.
        return -1;
    }
    
    bool ok = false;
    int number = dt.toInt(&ok);
    return ok ? number : -1;
}

QDomAttr
XmlData::attribute(const QString& p_name) const
{
    return attribute(p_name, m_node);
}

QDomAttr
XmlData::attribute(const QString& p_name, const QDomNode& p_node) const
{
    if (p_node.isNull()) {
        return QDomAttr();
    }
    
    QDomNamedNodeMap attributes = p_node.attributes();
    QDomNode attrNode = attributes.namedItem(p_name);
    if (!attrNode.isNull() && attrNode.isAttr()) {
        return attrNode.toAttr();
    }
    
    return QDomAttr();
}
