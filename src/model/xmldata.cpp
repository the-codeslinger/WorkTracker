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

XmlData::XmlData(QDomDocument* p_dataSource)
    : m_dataSource(p_dataSource)
{
}

XmlData::XmlData(QDomDocument* p_dataSource, QDomElement p_node)
    : m_dataSource(p_dataSource)
    , m_node(p_node)
{
}

XmlData::XmlData(const XmlData& p_other)
    : m_dataSource(p_other.m_dataSource)
    , m_node(p_other.m_node)
{
}

XmlData::XmlData(XmlData&& p_temp)
    : m_dataSource(p_temp.m_dataSource)
    , m_node(p_temp.m_node)
{
    p_temp.m_dataSource = nullptr;
}

QDomDocument* 
XmlData::dataSource() const
{
    return m_dataSource;
}

void
XmlData::setDataSource(QDomDocument* p_dataSource)
{
    m_dataSource = p_dataSource;
}

QDomNode
XmlData::node() const
{
    return m_node;
}

void
XmlData::setNode(QDomNode p_node)
{
    m_node = p_node.toElement();
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
}

bool 
XmlData::operator==(const XmlData& p_other) const
{
    return m_node == p_other.m_node;
}

XmlData&
XmlData::operator=(const XmlData& p_other)
{
    m_node = p_other.m_node;
}

void
XmlData::addAttribute(QString p_name, QString p_value)
{
    if (m_node.isNull()) {
        qDebug() << "node is null";
    }

    if (!p_value.isEmpty()) {
        QDomAttr attr = findAttribute(p_name).toAttr();
        if (attr.isNull()) {
            attr = m_dataSource->createAttribute(p_name);
            m_node.setAttributeNode(attr);
        }
        attr.setValue(p_value);
    }
}

QDomNode
XmlData::findAttribute(QString p_name) const
{
    return findAttributeFromNode(m_node, p_name);
}

QVariant
XmlData::attributeValue(QString p_name) const
{
    return attributeValueFromNode(m_node, p_name);
}

QDomNode
XmlData::findAttributeFromNode(QDomNode p_node, QString p_name) const
{
    QDomNamedNodeMap attributes = p_node.attributes();
    return attributes.namedItem(p_name);
}

QVariant
XmlData::attributeValueFromNode(QDomNode p_node, QString p_name) const
{
    QDomNode attrNode = findAttributeFromNode(p_node, p_name);
    if (attrNode.isNull() || !attrNode.isAttr()) {
        return QVariant();
    }

    QDomAttr attr = attrNode.toAttr();
    return QVariant(attr.value());
}
