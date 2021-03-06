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
