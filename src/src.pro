QT += core gui network xml

TARGET = TOBI_RemoteScope
TEMPLATE = app

OBJECTS_DIR = ../tmp/

include (../TOBI_RemoteScope_settings.pri)
include ($$_PRO_FILE_PWD_/config_widget/config_widget.pri)
include ($$_PRO_FILE_PWD_/data_collector/data_collector.pri)
include ($$_PRO_FILE_PWD_/connection_widget/connection_widget.pri)
include ($$_PRO_FILE_PWD_/visualisation/visualisation.pri)
include ($$_PRO_FILE_PWD_/base/base.pri)

SOURCES += $$_PRO_FILE_PWD_/main.cpp \
        $$_PRO_FILE_PWD_/main_window.cpp

HEADERS  += $$_PRO_FILE_PWD_/main_window.h

FORMS    += $$_PRO_FILE_PWD_/main_window.ui
