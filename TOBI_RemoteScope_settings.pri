CONFIG += no_keywords

INCLUDEPATH += $$PWD/external/include

win32 {
  INCLUDEPATH += C:/boost/1_49_0
}

RESOURCES += \
    ../resources.qrc

DEFINES += TIXML_USE_TICPP


HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    message(Building 64 bit )
  }else::{
    message(Building 32 bit )
  }


unix {
    LIBS += -lboost_thread -lboost_system

    HARDWARE_PLATFORM = $$system(uname -m)
    contains( HARDWARE_PLATFORM, x86_64 )::{
        # 64-bit Linux
        LIBS += -L$$PWD/external/lib/ticpp/linux  \
                -L$$PWD/external/lib/tia/linux/amd64 \\
                -Wl,-rpath=$$PWD/external/lib/tia/linux/amd64 \
                -ltia  -lticpp_64

    }else::{
        # 32-bit Linux
        LIBS += -L$$PWD/external/lib/ticpp/linux  \
                -L$$PWD/external/lib/tia/linux/x86 \
                -Wl,-rpath=$$PWD/external/lib/tia/linux/x86 \
                -ltia  -lticpp

    }
}

win32 {
  LIBS += $$PWD/external/lib/tia/win/Win32/tia.lib \
          $$PWD/external/lib/ticpp/win/Win32/libticpp.lib \
          -LC:/boost/1_49_0/lib_32\
          -llibboost_system-vc100-mt-s-1_49
}


