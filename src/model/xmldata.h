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

#ifndef XMLDATA_H
#define XMLDATA_H

#include <QString>
#include <QDomElement>

class QVariant;
class QDomNode;
class QDomDocument;

class XmlData
{
public:
    void setDataSource(QDomDocument* dataSource);
    QDomNode node() const;

    bool isNull() const;
    void clear();

protected:
    XmlData(QDomDocument* dataSource);
    XmlData(QDomDocument* dataSource, QDomElement node);
    XmlData(const XmlData& other);
    XmlData(XmlData&& temp);

    void addAttribute(QString name, QString value);
    QDomNode findAttribute(QString name) const;
    QVariant attributeValue(QString name) const;

    QDomNode findAttributeFromNode(QDomNode node, QString name) const;
    QVariant attributeValueFromNode(QDomNode node, QString name) const;

    QDomDocument* m_dataSource;
    QDomElement   m_node;
};

#endif // XMLDATA_H
