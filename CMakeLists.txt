cmake_minimum_required(VERSION 3.0)

project(pgadmin-qt)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(PkgConfig)
pkg_check_modules(PC_LIBPQ REQUIRED libpq)

if (PC_LIBPQ_FOUND)
    include_directories(${PC_LIBPQ_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Could not find libpq")
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_CXX_FLAGS "-ggdb")
endif()

message("==========================")
message(STATUS "Building ${PROJECT_NAME} in ${CMAKE_BUILD_TYPE}")
message("==========================")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE UI_FORMS *.ui)

set(SOURCES
    # db
    db/pgconnection.cpp
    db/pgproperties.cpp
    db/pgset.cpp

    # dialog
    dialog/connectiondialog.cpp

    # form
    form/pgadmin.cpp

    # schema
    schema/pgaccessmethod.cpp
    schema/pgcast.cpp
    schema/pgcollation.cpp
    schema/pgdatabase.cpp
    schema/pgeventtrigger.cpp
    schema/pgextension.cpp
    schema/pgforeigndatawrapper.cpp
    schema/pgforeignserver.cpp
    schema/pggroup.cpp
    schema/pglanguage.cpp
    schema/pgobject.cpp
    schema/pgrole.cpp
    schema/pgschema.cpp
    schema/pgserver.cpp
    schema/pgtablespace.cpp
    schema/pguser.cpp
    schema/pgusermapping.cpp

    # widget
    widget/objectbrowser.cpp
    widget/propertytable.cpp

    # resources
    resources/pgadmin.qrc

    # ui
    ${UI_FORMS}

    # headers
    ${HEADERS}
)

include_directories(include)

add_executable(pgadmin-qt main.cpp ${SOURCES})

target_link_libraries(pgadmin-qt
                      Qt5::Widgets
                      pq)

