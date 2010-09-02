TEMPLATE = subdirs

SUBDIRS = tests


INCLUDEPATH += $$_PRO_FILE_PWD_/external/include

LIBS += -L$$_PRO_FILE_PWD_/external/lib \
        -lssclient \
        -lboost_system \
        -lboost_date_time \
        -lticpp
