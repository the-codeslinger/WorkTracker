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

#ifndef WORKDAYLIST_H
#define WORKDAYLIST_H

#include "xmldata.h"
#include "datasource.h"

class WorkDay;

/*!
 * Wrapper around the DOM node of all work-day nodes in the database.
 */
class WorkDayList : public XmlData
{
public:
    /*!
     * Create a new work-day list. In order to use the list a data source must be set
     * first.
     */
    WorkDayList();

    /*!
     * Create a new work-day list.
     *
     * \param dataSource
     * The database from which the data is read.
     */
    WorkDayList(DataSource dataSource);
    
    /*!
     * Set the database from which the data is read.
     */
    void setDataSource(DataSource dataSource);

    /*!
     * \return
     * Returns the number of work-days in the list.
     */
    int size() const;

    /*!
     * \return
     * If the latest work-day has not been finished then it is returned. Otherwise a
     * null-object is returned.
     */
    WorkDay findLastOpen() const;

    /*!
     * Get a specific work-day from the list.
     *
     * \param index
     * The position of the work-day in the list.
     *
     * \return
     * Returns the requested work-day or a null-object if the index is out of bounds.
     */
    WorkDay at(int index) const;

private:
    void findOrCreateElement();
};

#endif
