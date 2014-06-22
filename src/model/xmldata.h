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

/*!
 * \brief Base class for models with direct access to the XML DOM.
 *
 * WorkTracker stores its data in an XML database file. The `XmlData` is the base class
 * for a direct representation of one piece of information in the XML file to the
 * application. The respective DOM node is kept as a reference and all information are
 * read from that node and immediately written to that node.
 *
 * Any model class that is a direct representative of an item in the DOM tree inherits
 * from this class. By that it gets access to getter and setter methods for DOM
 * manipulation.
 */
class XmlData
{
public:
    /*!
     * Set the data source from which to read the data.
     *
     * \param dataSource
     * A loaded XML file. Ownership of the data source stays with the calling code.
     */
    void setDataSource(QDomDocument* dataSource);

    /*!
     * \return
     * Returns the DOM node that is the base of the model entity.
     */
    QDomNode node() const;

    /*!
     * Set a new node.
     *
     * \param node
     * The new XML node for the model.
     */
    void setNode(QDomNode node);

    /*!
     * \return
     * Returns `true` if the node isn't yet valid or `false` if it is valid.
     */
    bool isNull() const;

    /*!
     * Deletes the data and releases the connection to the DOM tree.
     */
    void clear();

protected:
    /*!
     * Creates a new instance with a data source.
     */
    XmlData(QDomDocument* dataSource);

    /*!
     * Creates a new instance with a data source and a DOM node.
     */
    XmlData(QDomDocument* dataSource, QDomElement node);

    /*!
     * Copy the values from another instance. This is not a deep copy. In the end both
     * instances reference the same data and changes in one will appear on the other as
     * well.
     */
    XmlData(const XmlData& other);

    /*!
     * Move-constructor that takes the data from another, temporary instance.
     */
    XmlData(XmlData&& temp);

    /*!
     * Add a new attribute to the node or assign a new value to an already existing
     * attribute.
     *
     * \param name
     * The name of the attribute.
     *
     * \param value
     * The attribute's (new) value. If this is empty then the attribute is not created /
     * updated.
     */
    void addAttribute(QString name, QString value);

    /*!
     * Find an attribute on the node.
     *
     * \param name
     * The name of the attribute to find.
     *
     * \return
     * Returns a valid attribute node if found or a null-node.
     */
    QDomNode findAttribute(QString name) const;

    /*!
     * Get the value of an attribute.
     *
     * \param name
     * The name of the attribute to get the value from.
     *
     * \return
     * If the attribute exists its value is returned, otherwise a null-value.
     */
    QVariant attributeValue(QString name) const;

    /*!
     * This is similar to XmlData::findAttribute(QString) with the difference that this
     * method works on the node that is being passed to it rather than XmlData::m_node.
     *
     * \param node
     * The node on which to look for the attribute.
     *
     * \param name
     * The name of the attribute to find.
     *
     * \return
     * Returns a valid attribute node if found or a null-node.
     */
    QDomNode findAttributeFromNode(QDomNode node, QString name) const;

    /*!
     * This is similar to XmlData::attributeValue(QString) with the difference that this
     * method works on the node that is being passed to it rather than XmlData::m_node.
     *
     * \param name
     * The name of the attribute to get the value from.
     *
     * \return
     * If the attribute exists its value is returned, otherwise a null-value.
     */
    QVariant attributeValueFromNode(QDomNode node, QString name) const;

    /*!
     * The DOM document that represents the XML database file.
     */
    QDomDocument* m_dataSource;
    /*!
     * The link into the DOM tree that represents exactly one item of a specific (
     * arbitrarily complex) model class.
     */
    QDomElement m_node;
};

#endif // XMLDATA_H
