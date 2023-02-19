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
