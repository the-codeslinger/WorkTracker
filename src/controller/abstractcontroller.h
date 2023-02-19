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

#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include "../model/datasource.h"

#include <QString>

/*!
 * Base class for all controllers that share working on the XML database.
 */
class AbstractController
{
public:
    /*!
     * \return
     * Returns the currently set XML document.
     */
    DataSource dataSource() const;

protected:
    /*!
     * Constructor for derived classes that create the data source, either by loading
     * from a file or constructing it in memory.
     */
    AbstractController(QString databaseLocation);

    /*!
     * Constructor for derived classes that receive a data source as part of their 
     * construction.
     */
    AbstractController(DataSource dataSource);

    DataSource m_dataSource;
};

#endif
