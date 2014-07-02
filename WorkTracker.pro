# Copyright 2014 Robert Lohr
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET    = WorkTracker
TEMPLATE  = app
CONFIG   += c++11
RC_FILE   = WorkTracker.rc

SOURCES += src/main.cpp\
        src/worktracker.cpp \
    src/model/ui/tasklistmodel.cpp \
    src/model/task.cpp \
    src/model/worktask.cpp \
    src/model/workday.cpp \
    src/controller/worktrackercontroller.cpp \
    src/model/xmldata.cpp \
    src/aboutdialog.cpp \
    src/helper.cpp

HEADERS  += src/worktracker.h \
    src/model/ui/tasklistmodel.h \
    src/model/task.h \
    src/model/worktask.h \
    src/model/workday.h \
    src/controller/worktrackercontroller.h \
    src/model/xmldata.h \
    src/aboutdialog.h \
    src/helper.h

FORMS    += ui/worktracker.ui \
    ui/aboutdialog.ui

OTHER_FILES +=

RESOURCES += \
    icons.qrc
