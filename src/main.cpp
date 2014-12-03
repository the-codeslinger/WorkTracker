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

#include "ui/worktracker.h"
#include "controller/worktrackercontroller.h"
#include "model/worktask.h"

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
    
    qRegisterMetaType<WorkTask>("WorkTask");

    QString filePath;

    if (2 == argc) {
        qDebug() << "Use database file from parameters: " << argv[1];
        filePath = argv[1];
    }

    WorkTrackerController controller(filePath);
    WorkTracker w(&controller);
    w.show();

    int result = a.exec();

    controller.close();

    return result;
}
