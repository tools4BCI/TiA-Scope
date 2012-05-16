 

HEADERS += \
    $$PWD/reader_thread.h \
    $$PWD/fourier_transform_thread.h \
    data_collector/qt_tia_client/qt_tia_client.h \
    data_collector/qt_tia_client/impl/tia_client_version02.h \
    data_collector/filters.h \
    data_collector/filters/filter.h \
    data_collector/filters/butterworth_filter.h \
    data_collector/qt_tia_client/datapacket.h \
    data_collector/qt_tia_client/impl/tia_datapacket_version2.h \
    data_collector/qt_tia_client/tia_exception.h \
    data_collector/qt_tia_client/impl/tia_client_version10.h \
    data_collector/qt_tia_client/impl/tia_control_message.h \
    data_collector/qt_tia_client/impl/data_receive_blocker.h \
    data_collector/qt_tia_client/impl/line_receive_blocker.h \
    data_collector/qt_tia_client/impl/tia_datapacket_basedlibtia.h \
    data_collector/qt_tia_client/impl/tia_client_based_libtia.h

SOURCES += \
    $$PWD/reader_thread.cpp \
    $$PWD/fourier_transform_thread.cpp \
    data_collector/qt_tia_client/qt_tia_client.cpp \
    data_collector/qt_tia_client/impl/tia_client_version02.cpp \
    data_collector/filters.cpp \
    data_collector/filters/butterworth_filter.cpp \
    data_collector/qt_tia_client/impl/tia_datapacket_version2.cpp \
    data_collector/qt_tia_client/impl/tia_client_version10.cpp \
    data_collector/qt_tia_client/impl/data_receive_blocker.cpp \
    data_collector/qt_tia_client/impl/line_receive_blocker.cpp \
    data_collector/qt_tia_client/impl/tia_datapacket_basedlibtia.cpp \
    data_collector/qt_tia_client/impl/tia_client_based_libtia.cpp
