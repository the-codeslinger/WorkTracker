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

SOURCES += \
    src/main.cpp\
    src/helper.cpp \
    src/controller/abstractcontroller.cpp \
    src/controller/editorcontroller.cpp \
    src/controller/preferencescontroller.cpp \
    src/controller/worktrackercontroller.cpp \
    src/model/datasource.cpp \
    src/model/task.cpp \
    src/model/tasklist.cpp \
    src/model/workday.cpp \
    src/model/workdaylist.cpp \
    src/model/worktask.cpp \
    src/model/worktime.cpp \
    src/model/xmldata.cpp \
    src/ui/delegate/workdaydelegate.cpp \
    src/ui/delegate/taskdelegate.cpp \
    src/ui/model/tasklistmodel.cpp \
    src/ui/model/workdaymodel.cpp \
    src/ui/model/selectedworkdaymodel.cpp \
    src/ui/model/worktaskmodel.cpp \
    src/ui/aboutdialog.cpp \
    src/ui/selectworkdaypage.cpp \
    src/ui/editworktaskpage.cpp \
    src/ui/preferencesdialog.cpp \
    src/ui/selecttaskdialog.cpp \
    src/ui/editorwizard.cpp \
    src/ui/worktracker.cpp 

HEADERS  += \
    src/helper.h \
    src/controller/abstractcontroller.h \
    src/controller/worktrackercontroller.h \
    src/controller/editorcontroller.h \
    src/controller/preferencescontroller.h \
    src/model/datasource.h \
    src/model/task.h \
    src/model/tasklist.h \
    src/model/workday.h \
    src/model/workdaylist.h \
    src/model/worktask.h \
    src/model/worktime.h \
    src/model/xmldata.h \
    src/ui/delegate/workdaydelegate.h \
    src/ui/delegate/taskdelegate.h \
    src/ui/model/tasklistmodel.h \
    src/ui/model/workdaymodel.h \
    src/ui/model/selectedworkdaymodel.h \
    src/ui/model/worktaskmodel.h \
    src/ui/worktracker.h \
    src/ui/aboutdialog.h \
    src/ui/selectworkdaypage.h \
    src/ui/editworktaskpage.h \
    src/ui/preferencesdialog.h \
    src/ui/selecttaskdialog.h \
    src/ui/editorwizard.h

FORMS    += \
    ui/worktracker.ui \
    ui/aboutdialog.ui \
    ui/selectworkdaywidget.ui \
    ui/editworktaskwidget.ui \
    ui/preferencesdialog.ui \
    ui/selecttaskdialog.ui

OTHER_FILES +=

RESOURCES += \
    icons.qrc

TRANSLATIONS = l10n/de_DE.ts \
               l10n/en_US.ts

CODECFORSRC = UTF-8
