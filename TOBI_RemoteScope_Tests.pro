TEMPLATE += app
CONFIG += console
DESTDIR = tests

CONFIG   -= app_bundle

QT += network xml

TARGET = tobi_scope_tests

OBJECTS_DIR = tmp/tests/

#SUBDIRS = tests

SOURCES += \
    tests/main.cpp \
    tests/test_tia_based_datapacket.cpp \
    tests/test_tia_based_client.cpp

HEADERS += \
    tests/print_to_console.h

INCLUDEPATH += src \
    external/include \
    tests/UnitTest++

win32 {
  INCLUDEPATH += C:/boost/1_49_0
}

include(src/data_collector/data_collector.pri)
include(src/base/base.pri)

DEPENDPATH += $$INCLUDEPATH


DEFINES += TIXML_USE_TICPP


HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    message(Building 64 bit )
  }else::{
    message(Building 32 bit )
  }


LIBS += -Ltests/UnitTest++

unix {

    HARDWARE_PLATFORM = $$system(uname -m)
    contains( HARDWARE_PLATFORM, x86_64 )::{
        # 64-bit Linux
        LIBS += -Lexternal/lib/ticpp/linux  \
                -Lexternal/lib/tia/linux/amd64 \
                -Wl,-rpath=external/lib/tia/linux/amd64 \
                -ltia  -lticpp_64 \
                -lUnitTest++_64

    }else::{
        # 32-bit Linux
        LIBS += -Lexternal/lib/ticpp/linux  \
                -Lexternal/lib/tia/linux/x86 \
                -Wl,-rpath=external/lib/tia/linux/x86 \
                -ltia  -lticpp \
                -lUnitTest++

    }

    LIBS += -lboost_thread -lboost_system

}

win32 {
  LIBS += $$PWD/external/lib/tia/win/Win32/tia.lib \
          $$PWD/external/lib/ticpp/win/Win32/libticpp.lib \
          -LC:/boost/1_49_0/lib_32\
          -llibboost_system-vc100-mt-s-1_49
}
