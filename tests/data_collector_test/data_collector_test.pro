#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T08:38:23
#
#-------------------------------------------------

QT       += network xml testlib

TARGET = data_collector_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    test_tia_based_datapacket.cpp \
    test_tia_based_client.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += . \
    ../../src \
    ../../external/include \
    ../UnitTest++


include(../../src/data_collector/data_collector.pri)
include(../../src/base/base.pri)

DEPENDPATH += $$INCLUDEPATH


DEFINES += TIXML_USE_TICPP


HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    message(Building 64 bit )
  }else::{
    message(Building 32 bit )
  }


LIBS += -L../UnitTest++

unix {
    LIBS += -lboost_thread -lboost_system

    HARDWARE_PLATFORM = $$system(uname -m)
    contains( HARDWARE_PLATFORM, x86_64 )::{
        # 64-bit Linux
        LIBS += -L../../external/lib/ticpp/linux  \
                -L../../external/lib/tia/linux/amd64 \
                -ltia  -lticpp_64 \
                -lUnitTest++_64

    }else::{
        # 32-bit Linux
        LIBS += -L../../external/lib/ticpp/linux  \
                -L../../external/lib/tia/linux/x86 \
                -Wl,-rpath=../../external/lib/tia/linux/x86 \
                -ltia  -lticpp \
                -lUnitTest++

    }
}

win32:LIBS += ../../lib/tia.lib

HEADERS += \
    print_to_console.h

