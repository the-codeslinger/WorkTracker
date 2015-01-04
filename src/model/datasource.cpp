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

#include "datasource.h"

#include <QString>
#include <QStandardPaths>
#include <QDomNode>
#include <QDomElement>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <algorithm>

DataSource::DataSource(const QString& location)
    : m_location(location)
{ }

DataSource::DataSource(const DataSource& other)
    : m_dom(other.m_dom)
    , m_location(other.m_location)
{ }

DataSource::DataSource(DataSource&& temp)
    : m_dom(std::move(temp.m_dom))
    , m_location(std::move(temp.m_location))
{ }

DataSource& 
DataSource::operator=(const DataSource& other)
{
    m_dom = other.m_dom;
    m_location = other.m_location;
    return *this;
}

DataSource& 
DataSource::operator=(DataSource&& temp)
{
    m_dom = std::move(temp.m_dom);
    m_location = std::move(temp.m_location);
    return *this;
}

bool
DataSource::isNull() const
{
    return m_dom.isNull();
}

QDomDocument
DataSource::document() const
{
    return m_dom;
}

bool
DataSource::load() 
{
    if (m_location.isEmpty())  {
        // Set up the data source for our application, i.e. load an existing database or,
        // e.g. on the first start, create a new database.
        QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

        QDir dir(path);
        if (!dir.exists()) {
            if (dir.mkpath(path)) {
                qDebug() << "Cannot create database dir: " << path;
                return false;
            }
        }
        m_location = path + "/Database.xml";
    }

    QFile xmlFile(m_location);
    if (!m_dom.setContent(&xmlFile)) {
        qDebug() << "No database file found; Create new";

        QString procInstruction = "version=\"1.0\" encoding=\"UTF-8\"";
        QDomNode xmlNode = m_dom.createProcessingInstruction("xml", procInstruction);
        m_dom.appendChild(xmlNode);

        QDomElement root = m_dom.createElement("worktracker");
        m_dom.appendChild(root);

        QDomElement version = m_dom.createElement("version");
        version.appendChild(m_dom.createTextNode("1.0"));
        root.appendChild(version);

        root.appendChild(m_dom.createElement("tasks"));
        root.appendChild(m_dom.createElement("workdays"));
    }
    xmlFile.close();

    return true;
}

bool
DataSource::save() const
{
    if (!m_dom.isNull()) {
        QFile xmlFile(m_location);
        if (!xmlFile.open(QIODevice::WriteOnly)) {
            qDebug() << "Cannot open XML file for writing.";
            return false;
        }
        else {
            QTextStream out(&xmlFile);
            out.setCodec("UTF-8");
            m_dom.save(out, 2, QDomNode::EncodingFromTextStream);
            xmlFile.close();
            return true;
        }
    }

    return false;
}
