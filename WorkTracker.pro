#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T18:40:09
#
#-------------------------------------------------

QT       += core gui xml webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkTracker
TEMPLATE = app


SOURCES += main.cpp\
        worktracker.cpp \
    tasklistmodel.cpp \
    task.cpp \
    tasklist.cpp \
    worktask.cpp \
    workday.cpp \
    workdaylist.cpp

HEADERS  += worktracker.h \
    tasklistmodel.h \
    task.h \
    tasklist.h \
    worktask.h \
    workday.h \
    workdaylist.h

FORMS    += worktracker.ui
