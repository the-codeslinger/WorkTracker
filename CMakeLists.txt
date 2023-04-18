# Generated from WorkTracker.pro.

cmake_minimum_required(VERSION 3.16)
project(WorkTracker VERSION 1.5.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Xml Widgets)
#qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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

set(l10n_files
    "${CMAKE_SOURCE_DIR}/l10n/qt_de_DE.qm"
    "${CMAKE_SOURCE_DIR}/l10n/de_DE.qm"
    "${CMAKE_SOURCE_DIR}/l10n/en_US.qm"
)

get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

# Create application bundles for release on GitHub.
IF(WIN32)
    add_executable(WorkTracker "")
    target_sources(WorkTracker
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/WorkTracker.rc"
    )

    set_target_properties(WorkTracker PROPERTIES
        WIN32_EXECUTABLE ON
    )

    # Copy language files to build directory.
    set(l10n_out_dir "${CMAKE_CURRENT_BINARY_DIR}/l10n")
    message("${l10n_out_dir}/")
    file(MAKE_DIRECTORY "${l10n_out_dir}/")
    foreach(l10n_file ${l10n_files}) 
        file(COPY ${l10n_file} DESTINATION ${l10n_out_dir})
    endforeach()

    # Run windeployqt to gather all Qt and runtime libraries.
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET WorkTracker POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"
        ARGS "WorkTracker.exe" "--no-translations"
        COMMENT "Execute windeployqt to create Windows release")
ELSEIF(APPLE)
    # https://doc-snapshots.qt.io/qt6-dev/appicon.html#using-cmake-on-macos
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    set(app_icon_macos "${CMAKE_SOURCE_DIR}/icon/icon.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(${l10n_files} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/l10n")

    qt_add_executable(WorkTracker MACOSX_BUNDLE "" ${app_icon_macos} ${l10n_files})

    set_target_properties(WorkTracker PROPERTIES
        MACOSX_BUNDLE ON
    )

    # https://riptutorial.com/qt/example/24133/integrating-with-cmake
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET WorkTracker POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
        ARGS "WorkTracker.app" "-dmg"
        COMMENT "Execute macdeployqt to create macOS bundle")
ELSE()
    add_executable(WorkTracker ${worktracker_src})
ENDIF()

target_sources(WorkTracker
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
        
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/abstractcontroller.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/abstractcontroller.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/editorcontroller.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/editorcontroller.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/preferencescontroller.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/preferencescontroller.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/worktrackercontroller.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/worktrackercontroller.h"
        
        "${CMAKE_CURRENT_SOURCE_DIR}/src/helper.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/helper.h"
        
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/datasource.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/datasource.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/task.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/task.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/tasklist.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/tasklist.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/workday.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/workday.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/workdaylist.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/workdaylist.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/worktask.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/worktask.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/worktime.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/worktime.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/xmldata.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/xmldata.h"
        
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/aboutdialog.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/aboutdialog.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/completerlineedit.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/completerlineedit.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/delegate/taskdelegate.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/delegate/taskdelegate.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/delegate/workdaydelegate.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/delegate/workdaydelegate.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editorwizard.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editorwizard.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editworktaskpage.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editworktaskpage.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/selectedworkdaymodel.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/selectedworkdaymodel.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/tasklistmodel.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/tasklistmodel.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/workdaymodel.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/workdaymodel.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/worktaskmodel.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/model/worktaskmodel.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/preferencesdialog.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/preferencesdialog.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/selecttaskdialog.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/selecttaskdialog.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/selectworkdaypage.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/selectworkdaypage.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/worktracker.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/worktracker.h"
        
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/aboutdialog.ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editworktaskwidget.ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/preferencesdialog.ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/selecttaskdialog.ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/selectworkdaywidget.ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/worktracker.ui"
)

qt6_add_resources(WorkTracker "icon"
    PREFIX
        "/"
    FILES
        ${icons_resource_files}
)

target_include_directories(WorkTracker PUBLIC
    src
)

target_link_libraries(WorkTracker PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Xml
    Qt::Widgets
)
