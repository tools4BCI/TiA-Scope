CONFIG += no_keywords

INCLUDEPATH += $$PWD/external/include

LIBS += -ltiaclient -lticpp

unix {
    LIBS += -lboost_thread \
            -lboost_system

    HARDWARE_PLATFORM = $$system(uname -m)

    contains( HARDWARE_PLATFORM, x86_64 ) {
        LIBS += -L$$PWD/external/lib/linux64
    } else {
        LIBS += -L$$PWD/external/lib/linux32
    }
}
win32 {
    DEFINES += _WIN32_WINNT=0x0501 \
               WIN32
    LIBS += -L$$PWD/external/lib/win32 \
            $$PWD/external/lib/win32/libboost_system-mgw44-mt-1_42.lib \
            $$PWD/external/lib/win32/libboost_date_time-mgw44-mt-1_42.lib \
            -lws2_32
}


RESOURCES += \
    ../resources.qrc
