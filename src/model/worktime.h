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

#ifndef WORKTIME_H
#define WORKTIME_H

#include "xmldata.h"
#include "datasource.h"

/*!
 * Implements the DOM element of the work-task that contains the start and stop time.
 */
class WorkTime : public XmlData
{
public:
    /*!
     * Creates an empty instance. Calling `WorkTime::isNull` returns `true`.
     */
    WorkTime();
    
    /*!
     * Creates a new instance from the DOM element.
     */
    WorkTime(DataSource dataSource, const QDomElement& element);
    
    /*!
     * Creates a new instance with the start and stop time.
     */
    WorkTime(DataSource dataSource, const QDateTime& start, const QDateTime& stop);
    
    /*!
     * Copy constructor.
     */
    WorkTime(const WorkTime& other);
    
    /*!
     * \return 
     * Returns the start time if it is set. Otherwise the date-time returned is null.
     */
    QDateTime start() const;
    
    /*!
     * Set the start date-time.
     * 
     * \param start
     * The new start value. If is is null then the DOM element is cleared.
     */
    void setStart(const QDateTime& start);
    
    /*!
     * \return 
     * Returns the stop time if it is set. Otherwise the date-time returned is null.
     */
    QDateTime stop() const;
    
    /*!
     * Set the stop date-time.
     * 
     * \param stop
     * The new stop value. If is is null then the DOM element is cleared.
     */
    void setStop(const QDateTime& stop);
    
    /*!
     * \return 
     * Returns the total amount of seconds between start and stop.
     */
    qint64 timeInSeconds() const;

protected:
    QString elementName() const;
};

#endif // WORKTIME_H
