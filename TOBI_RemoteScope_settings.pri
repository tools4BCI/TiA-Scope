CONFIG += no_keywords

INCLUDEPATH += $$PWD/external/include \
               $$PWD/src

LIBS += -L$$PWD/external/lib \
        -lssclient \
        -lboost_system \
        -lboost_date_time \
        -lticpp
