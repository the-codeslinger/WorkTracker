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

XmlData::XmlData(QDomDocument* dataSource)
    : m_dataSource(dataSource)
{
}

XmlData::XmlData(QDomDocument* dataSource, QDomElement node)
    : m_dataSource(dataSource)
    , m_node(node)
{
}

XmlData::XmlData(const XmlData& other)
    : m_dataSource(other.m_dataSource)
    , m_node(other.m_node)
{
}

XmlData::XmlData(XmlData&& temp)
    : m_dataSource(temp.m_dataSource)
    , m_node(temp.m_node)
{
    temp.m_dataSource = nullptr;
}

void
XmlData::setDataSource(QDomDocument* dataSource)
{
    m_dataSource = dataSource;
}

QDomNode
XmlData::node() const
{
    return m_node;
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

void
XmlData::addAttribute(QString name, QString value)
{
    if (m_node.isNull()) {
        qDebug() << "node is null";
    }

    if (!value.isEmpty()) {
        QDomAttr attr = findAttribute(name).toAttr();
        if (attr.isNull()) {
            attr = m_dataSource->createAttribute(name);
            m_node.setAttributeNode(attr);
        }
        attr.setValue(value);
    }
}

QDomNode
XmlData::findAttribute(QString name) const
{
    return findAttributeFromNode(m_node, name);
}

QVariant
XmlData::attributeValue(QString name) const
{
    return attributeValueFromNode(m_node, name);
}

QDomNode
XmlData::findAttributeFromNode(QDomNode node, QString name) const
{
    QDomNamedNodeMap attributes = node.attributes();
    return attributes.namedItem(name);
}

QVariant
XmlData::attributeValueFromNode(QDomNode node, QString name) const
{
    QDomNode attrNode = findAttributeFromNode(node, name);
    if (attrNode.isNull() || !attrNode.isAttr()) {
        return QVariant();
    }

    QDomAttr attr = attrNode.toAttr();
    return QVariant(attr.value());
}
