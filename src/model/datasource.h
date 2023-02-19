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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QString>
#include <QDomDocument>

class QFile;

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

    void makeDatabaseFilename();
    void createEmptyInMemoryDatabase();
    bool readDatabase(QFile& xmlFile);
};

#endif
