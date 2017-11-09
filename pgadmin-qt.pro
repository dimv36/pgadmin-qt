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
        form/pgadmin.cpp \
        dialog/connectiondialog.cpp \
        db/pgconnection.cpp \
        db/pgset.cpp \
        db/pgproperties.cpp \
        schema/pgobject.cpp \
        schema/pgserver.cpp \
        schema/pgdatabase.cpp \
        schema/pguser.cpp \
        schema/pgtablespace.cpp \
        schema/pggroup.cpp \
        schema/pgrole.cpp \
        schema/pgaccessmethod.cpp \
        schema/pgcast.cpp \
        schema/pgeventtrigger.cpp \
        schema/pgextension.cpp \
        schema/pglanguage.cpp \
        schema/pgschema.cpp \
        widget/objectbrowser.cpp \
        widget/propertytable.cpp

HEADERS += \
        include/form/pgadmin.h \
        include/dialog/connectiondialog.h \
        include/db/pgconnection.h \
        include/db/pgset.h \
        include/db/pgproperties.h \
        include/schema/pgobject.h \
        include/schema/pgserver.h \
        include/schema/pgdatabase.h \
        include/schema/pguser.h \
        include/schema/pgtablespace.h \
        include/schema/pggroup.h \
        include/schema/pgrole.h \
        include/schema/pgaccessmethod.h \
        include/schema/pgcast.h \
        include/schema/pgeventtrigger.h \
        include/schema/pgextension.h \
        include/schema/pglanguage.h \
        include/schema/pgschema.h \
        include/widget/objectbrowser.h \
        include/widget/propertytable.h \
        include/enums.h \
        include/settings.h \
        include/includes.h

FORMS += \
        ui/pgadmin.ui \
        ui/connectiondialog.ui

DISTFILES +=

RESOURCES += \
        resources/pgadmin.qrc

INCLUDEPATH += include

PG_CONFIG = $$system("which pg_config")
message(PG_CONFIG: "$$PG_CONFIG")

#isEmpty(PG_CONFIG)
#{
#    error("Need pg_config at PATH")
#}

INCLUDEPATH += $$system("pg_config --includedir")
LIBS += -lpq

