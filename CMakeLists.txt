# Generated from WorkTracker.pro.

cmake_minimum_required(VERSION 3.16)
project(WorkTracker VERSION 1.5.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Xml Widgets)
qt_standard_project_setup()

set(worktracker_src
    src/controller/abstractcontroller.cpp src/controller/abstractcontroller.h
    src/controller/editorcontroller.cpp src/controller/editorcontroller.h
    src/controller/preferencescontroller.cpp src/controller/preferencescontroller.h
    src/controller/worktrackercontroller.cpp src/controller/worktrackercontroller.h
    src/helper.cpp src/helper.h
    src/main.cpp
    src/model/datasource.cpp src/model/datasource.h
    src/model/task.cpp src/model/task.h
    src/model/tasklist.cpp src/model/tasklist.h
    src/model/workday.cpp src/model/workday.h
    src/model/workdaylist.cpp src/model/workdaylist.h
    src/model/worktask.cpp src/model/worktask.h
    src/model/worktime.cpp src/model/worktime.h
    src/model/xmldata.cpp src/model/xmldata.h
    src/ui/aboutdialog.cpp src/ui/aboutdialog.h
    src/ui/completerlineedit.cpp src/ui/completerlineedit.h
    src/ui/delegate/taskdelegate.cpp src/ui/delegate/taskdelegate.h
    src/ui/delegate/workdaydelegate.cpp src/ui/delegate/workdaydelegate.h
    src/ui/editorwizard.cpp src/ui/editorwizard.h
    src/ui/editworktaskpage.cpp src/ui/editworktaskpage.h
    src/ui/model/selectedworkdaymodel.cpp src/ui/model/selectedworkdaymodel.h
    src/ui/model/tasklistmodel.cpp src/ui/model/tasklistmodel.h
    src/ui/model/workdaymodel.cpp src/ui/model/workdaymodel.h
    src/ui/model/worktaskmodel.cpp src/ui/model/worktaskmodel.h
    src/ui/preferencesdialog.cpp src/ui/preferencesdialog.h
    src/ui/selecttaskdialog.cpp src/ui/selecttaskdialog.h
    src/ui/selectworkdaypage.cpp src/ui/selectworkdaypage.h
    src/ui/worktracker.cpp src/ui/worktracker.h
    src/ui/aboutdialog.ui
    src/ui/editworktaskwidget.ui
    src/ui/preferencesdialog.ui
    src/ui/selecttaskdialog.ui
    src/ui/selectworkdaywidget.ui
    src/ui/worktracker.ui
)

IF (WIN32)
    list(APPEND worktracker_src "WorkTracker.rc")
ENDIF()

add_executable(WorkTracker ${worktracker_src})

target_include_directories(WorkTracker PUBLIC
    src
)

target_link_libraries(WorkTracker PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Xml
    Qt::Widgets
)

set_target_properties(WorkTracker PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# Resources:
set(icons_resource_files
    "icon/About-Qt.svg"
    "icon/About.svg"
    "icon/Add.svg"
    "icon/Edit.svg"
    "icon/Flag_de_DE.svg"
    "icon/Flag_en_US.svg"
    "icon/Icon128.png"
    "icon/Icon16.png"
    "icon/Preferences.svg"
    "icon/Quit.svg"
    "icon/Remove.svg"
    "icon/Select.svg"
    "icon/Start-Day.svg"
    "icon/Start-Task.svg"
    "icon/Stop-Day.svg"
    "icon/Stop-Task.svg"
    "icon/Summary.svg"
)

qt6_add_resources(WorkTracker "icons"
    PREFIX
        "/"
    FILES
        ${icons_resource_files}
)

get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

# Create application bundles for release on GitHub.
IF(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

ELSEIF(APPLE)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

    add_custom_command(TARGET WorkTracker POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
        ARGS "WorkTracker.app" "-dmg"
        COMMENT "Execute macdeployqt to create macOS bundle")
ENDIF()
