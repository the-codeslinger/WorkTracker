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

#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include "../model/datasource.h"

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
    AbstractController();

    /*!
     * Constructor for derived classes that receive a data source as part of their 
     * construction.
     */
    AbstractController(DataSource dataSource);

    DataSource m_dataSource;
};

#endif