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

#include "worktracker.h"
#include "controller/worktrackercontroller.h"

#include <QApplication>
#include <QStandardPaths>
#include <QString>
#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QFile>
#include <QDir>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Typical Nerd");
    a.setApplicationName("WorkTracker");

    QString filePath;

    if (2 == argc) {
        qDebug() << "Use database file from parameters: " << argv[1];
        filePath = argv[1];
    }
    else {
        // Set up the data source for our application, i.e. load an existing database or,
        // e.g. on the first start, create a new database.
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        qDebug() << "Write database to " << dataPath;

        QDir dir(dataPath);
        if (!dir.exists()) {
            if (!dir.mkpath(dataPath)) {
                qDebug() << "Cannot create dir " << dataPath
                         << ". Database cannot be saved";
            }
        }
        filePath = dataPath + "/Database.xml";
    }

    QFile xmlFile(filePath);

    QDomDocument dataSource;
    if (!dataSource.setContent(&xmlFile)) {
        qDebug() << "No database file found; Create new";

        QString procInstruction = "version=\"1.0\" encoding=\"UTF-8\"";
        QDomNode xmlNode = dataSource.createProcessingInstruction("xml", procInstruction);
        dataSource.appendChild(xmlNode);

        QDomElement root = dataSource.createElement("worktracker");
        dataSource.appendChild(root);

        QDomElement version = dataSource.createElement("version");
        version.appendChild(dataSource.createTextNode("1.0"));
        root.appendChild(version);

        root.appendChild(dataSource.createElement("tasks"));
        root.appendChild(dataSource.createElement("workdays"));
    }
    xmlFile.close();

    WorkTrackerController controller(&dataSource);
    WorkTracker w(&controller);
    
    controller.setUi(&w);
    controller.run();

    int result = a.exec();

    controller.close();

    // Save the database now that the application is finished
    if (!xmlFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open database for writing; Save current data not possible";
    }
    else {
        QTextStream out(&xmlFile);
        out.setCodec("UTF-8");
        dataSource.save(out, 2, QDomNode::EncodingFromTextStream);
        xmlFile.close();
    }

    return result;
}
