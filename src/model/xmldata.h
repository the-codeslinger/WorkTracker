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

#include "datasource.h"

#include <QDomElement>
#include <QDomDocument>

class QString;
class QDate;
class QDateTime;

/*!
 * \brief Base class for models with direct access to the XML DOM.
 *
 * WorkTracker stores its data in an XML database file. The `XmlData` is the base class
 * for a direct representation of one piece of information in the XML file to the
 * application. The respective DOM element is kept as a reference and all information are
 * read from that element and immediately written to that element.
 *
 * Any model class that is a direct representative of an item in the DOM tree inherits
 * from this class. By that it gets access to getter and setter methods for DOM
 * manipulation.
 */
class XmlData
{
public:
    static const int invalidId = -1;

    /*!
     * \return 
     * Returns the data source of this XML data object. Do not delete the `QDomDocument`
     * instance.
     */
    DataSource dataSource() const;
    
    /*!
     * Set the data source from which to read the data.
     *
     * \param dataSource
     * A loaded XML file. Ownership of the data source stays with the calling code.
     */
    void setDataSource(const DataSource& dataSource);

    /*!
     * \return
     * Returns the DOM element that is the base of the model entity.
     */
    QDomElement element() const;

    /*!
     * Set a new element.
     *
     * \param element
     * The new XML element for the model.
     */
    void setElement(const QDomElement& element);
    
    /*!
     * \return 
     * Returns the element's parent element. If the element doesn't have a parent then 
     * `QDomNode::isNull()` will return `true`.
     */
    QDomNode parent() const;
    
    /*!
     * Appends a child to the current element. This basically makes the current element 
     * the parent.
     */
    void appendChild(const XmlData& child);

    /*!
     * \return
     * Returns `true` if the element isn't yet valid or `false` if it is valid.
     */
    bool isNull() const;

    /*!
     * Deletes the data and releases the connection to the DOM tree.
     */
    void clear();
    
    /**
     * Two `XmlData` instances  are considered equal if they refer to the same DOM element.
     *
     * @param other
     * The other `XmlData` to compare to.
     *
     * @return
     * `true` if both refer to the same DOM element or `false` if not.
     */
    bool operator==(const XmlData& other) const;
    
    /**
     * Two `XmlData` instances  are considered not equal if they don't refer to the same 
     * DOM element.
     *
     * @param other
     * The other `XmlData` to compare to.
     *
     * @return
     * `true` if both refer to the same DOM element or `false` if not.
     */
    bool operator!=(const XmlData& other) const;

protected:
    /*!
     * Default constructur. `XmlData::isNull()` will return `true`.
     */
    XmlData();

    /*!
     * Creates a new instance with a data source and a DOM element with the specified 
     * name.
     */
    XmlData(DataSource dataSource, const QString& name);

    /*!
     * Creates a new instance with a data source and a DOM element.
     */
    XmlData(DataSource dataSource, const QDomElement& element);

    /*!
     * Copy the values from another instance. This is not a deep copy. In the end both
     * instances reference the same data and changes in one will appear on the other as
     * well.
     */
    XmlData(const XmlData& other);
    
    /*!
     * Assigns the DOM element of the other `XmlData` instance to the current one.
     *
     * \param p_other
     * The other `XmlData` from which to copy the element.
     *
     * \return
     * Returns the changed current instance.
     */
    XmlData& operator=(const XmlData& other);

    /*!
     * Gets a current element's attribute value as a string. 
     */
    QString attributeString(const QString& name) const;
    
    /*!
     * Gets a current element's attribute value as a date-time. 
     */
    QDateTime attributeDateTime(const QString& name) const;
    
    /*!
     * Gets a current element's attribute value as a date. 
     */
    QDate attributeDate(const QString& name) const;
    
    /*!
     * Gets a current element's attribute value as an integer. 
     */
    int attributeInt(const QString& name) const;

    /*!
     * Add a new attribute to the element or assign a new value to an already existing
     * attribute.
     *
     * \param name
     * The name of the attribute.
     *
     * \param value
     * The attribute's (new) value.
     */
    void setAttribute(const QString& name, const QString& value);
    
    /*!
     * Add a new attribute to the element or assign a new value to an already existing
     * attribute.
     *
     * \param name
     * The name of the attribute.
     *
     * \param value
     * The attribute's (new) date-time value. If this is an invalid date then the 
     * attribute will be set to an empty value.
     */
    void setAttribute(const QString& name, const QDateTime& value);
    
    /*!
     * Add a new attribute to the element or assign a new value to an already existing
     * attribute.
     *
     * \param name
     * The name of the attribute.
     *
     * \param value
     * The attribute's (new) date value. If this is an invalid date then the attribute 
     * will be set to an empty value.
     */
    void setAttribute(const QString& name, const QDate& value);
    
    /*!
     * Add a new attribute to the element or assign a new value to an already existing
     * attribute.
     *
     * \param name
     * The name of the attribute.
     *
     * \param value
     * The attribute's (new) integer value. If this is -1 then the attribute is not 
     * created / updated.
     */
    void setAttribute(const QString& name, int value);

    /*!
     * Needs to be overwritten by derived classes which then return the name of the
     * XML element that is created when a `XmlData::setDataSource(const QDomDocument&)`
     * is called.
     */
    QString elementName() const { return ""; }

    /*!
     * The DOM document that represents the XML database file.
     */
    DataSource m_dataSource;
    /*!
     * The link into the DOM tree that represents exactly one item of a specific (
     * arbitrarily complex) model class.
     */
    QDomElement m_element;
};

#endif // XMLDATA_H
