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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QString>
#include <QDomDocument>

/*!
 * Represents the XML database file loaded into memory. If the file doesn't exist it
 * will be created.
 */
class DataSource
{
public:
    /*!
     * Creates a new data source with an optional path to the database file.
     *
     * \param[in] location
     * If specified the XML file at that location will be used as database file. 
     * Other the standard location in the user's data directory is used.
     */
    DataSource(QString location = "");

    /*!
     * Copy-constructor.
     */
    DataSource(const DataSource& other);

    /*!
     * Move-constructor.
     */
    DataSource(DataSource&& temp);
    
    /*!
     * Assignment operator.
     */
    DataSource& operator=(const DataSource& other);
    
    /*!
     * AMove-asignment operator.
     */
    DataSource& operator=(DataSource&& temp);
    
    /*!
     * \return
     * Returns the state of the data source. If it is correctly loaded then `false` is
     * returned. If there's a problem loading or creating the database then `true` is
     * returned.
     */
    bool isNull() const;

    /*!
     * \return
     * Returns the internal DOM document for use when DOM objects need to be created.
     */
    QDomDocument document() const;

    /*!
     * Reads the XML database from the given location, the default location in the user's
     * data directory or creates a new one in memory which will be written to the default
     * location.
     * 
     * \return
     * `true` on success or `false` if any error occurred.
     */
    bool load();

    /*!
     * Saves the database to the location determined in `DataSource::load()`.
     * 
     * \return
     * `true` on success or `false` if any error occurred.
     */
    bool save() const;

private:
    QDomDocument m_dom;
    QString      m_location;
};

#endif
