CONFIG += no_keywords

INCLUDEPATH += $$PWD/external/include

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

LIBS += -ltiaclient -lticpp

RESOURCES += \
    ../resources.qrc
