cmake_minimum_required(VERSION 3.16)

project(todo_app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(todo_app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        database.h database.cpp
        task.h
        addtask.h addtask.cpp
        deletebuttondelegate.h deletebuttondelegate.cpp
        taskui.h taskui.cpp
        icons.qrc
        taskinfo.h taskinfo.cpp
        edittask.h edittask.cpp
        categoryeditor.h categoryeditor.cpp
        choosecategory.h choosecategory.cpp
        categoryitemwidget.h categoryitemwidget.cpp
        choosepriority.h choosepriority.cpp
        priorityitemwidget.h priorityitemwidget.cpp
        flowlayout.h flowlayout.cpp
        indexwnd.h indexwnd.cpp
        calendarwnd.h calendarwnd.cpp
        navigationbar.h navigationbar.cpp
        calendarwidget.h calendarwidget.cpp
        appusagetracker.h appusagetracker.cpp
        focusmodewnd.h focusmodewnd.cpp
        circulartimerwidget.h circulartimerwidget.cpp
        appusageitem.h appusageitem.cpp
        login.h login.cpp
        intro.h intro.cpp
        createaccount.h createaccount.cpp
        profilewnd.h profilewnd.cpp
        profilesettingswidget.h profilesettingswidget.cpp
        changeaccountnamewnd.h changeaccountnamewnd.cpp
        changepasswnd.h changepasswnd.cpp
        changeimgwnd.h changeimgwnd.cpp

        UserSession.h
        UserSession.cpp
        appicon.rc
        changelanguagewnd.h changelanguagewnd.cpp
        taskmanager.h taskmanager.cpp
        aboutuswnd.h aboutuswnd.cpp
        helpandfeedbackwnd.h helpandfeedbackwnd.cpp
        thememanager.h thememanager.cpp
        themedialog.h themedialog.cpp
        warningwnd.h warningwnd.cpp
        customcalendar.h customcalendar.cpp
        timepickerwidget.h timepickerwidget.cpp
        deletetaskwnd.h deletetaskwnd.cpp
        logownd.h logownd.cpp
        infofornewuserwnd.h infofornewuserwnd.cpp

    )
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Sql)

target_link_libraries(todo_app
    PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::Sql
    shell32
    psapi
)


# Укажи Qt модули
find_package(Qt6 REQUIRED COMPONENTS LinguistTools Widgets)

# Создаём .qm файлы из .ts
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(todo_app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.todo_app)
endif()
set_target_properties(todo_app PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS todo_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(todo_app)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# или для Qt5:
# find_package(Qt5 REQUIRED COMPONENTS Widgets Sql)

target_link_libraries(todo_app PRIVATE Qt6::Widgets Qt6::Sql)


