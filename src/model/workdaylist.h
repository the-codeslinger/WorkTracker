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
