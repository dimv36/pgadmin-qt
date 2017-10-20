#-------------------------------------------------
#
# Project created by QtCreator 2017-10-14T20:26:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pgadmin-qt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        pgadmin.cpp \
    pgobject.cpp \
    objectbrowser.cpp \
    connectiondialog.cpp \
    pgserver.cpp \
    pgconnection.cpp \
    propertytable.cpp \
    pgset.cpp

HEADERS += \
        pgadmin.h \
    pgobject.h \
    enums.h \
    objectbrowser.h \
    connectiondialog.h \
    pgserver.h \
    pgconnection.h \
    settings.h \
    propertytable.h \
    pgset.h

FORMS += \
        pgadmin.ui \
    connectiondialog.ui

DISTFILES +=

RESOURCES += \
    pgadmin.qrc

PG_CONFIG = $$system("which pg_config")
message(PG_CONFIG: "$$PG_CONFIG")

#isEmpty(PG_CONFIG)
#{
#    error("Need pg_config at PATH")
#}

INCLUDEPATH += $$system("pg_config --includedir")
LIBS += -lpq

