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

class WorkDay;

class WorkDayList : public XmlData
{
public:
    WorkDayList();
    WorkDayList(const QDomDocument& dataSource);
    
    void setDataSource(const QDomDocument& dataSource);

    int size() const;
    WorkDay findLastOpen() const;
    WorkDay at(int index) const;

private:
    void findOrCreateElement();
};

#endif