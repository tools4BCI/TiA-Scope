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

#copy configuration file to output folder
OTHER_FILES += \
    tiascope.ini

ESCAPED_PWD = $$PWD
win32:ESCAPED_PWD ~= s,/,\\,g
ESCAPED_OUT_PWD = $$OUT_PWD
win32:ESCAPED_OUT_PWD ~= s,/,\\,g

win32 {
    copyfiles.commands += @echo NOW COPYING initialization files &
    copyfiles.commands += @call copy /Y $$quote($$ESCAPED_PWD\\tiascope.ini) $$quote($$ESCAPED_OUT_PWD\\)
}

unix {
    copyfiles.commands += echo NOW COPYING initialization files &
    copyfiles.commands += cp $$quote($$ESCAPED_PWD/tiascope.ini) $$quote($$ESCAPED_OUT_PWD\\)
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
