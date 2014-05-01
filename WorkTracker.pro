#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T18:40:09
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkTracker
TEMPLATE = app


SOURCES += src/main.cpp\
        src/worktracker.cpp \
    src/model/ui/tasklistmodel.cpp \
    src/model/task.cpp \
    src/model/worktask.cpp \
    src/model/workday.cpp \
    src/model/workdaylist.cpp \
    src/controller/worktrackercontroller.cpp

HEADERS  += src/worktracker.h \
    src/model/ui/tasklistmodel.h \
    src/model/task.h \
    src/model/worktask.h \
    src/model/workday.h \
    src/model/workdaylist.h \
    src/controller/worktrackercontroller.h

FORMS    += ui/worktracker.ui
