CONFIG += no_keywords

INCLUDEPATH += $$PWD/external/include \
               $$PWD/src

LIBS += -L$$PWD/external/lib \
        -ltiaclient \
        /usr/lib/libboost_system.a \
        /usr/lib/libboost_date_time.a \
        -lticpp

RESOURCES += \
    ../resources.qrc
