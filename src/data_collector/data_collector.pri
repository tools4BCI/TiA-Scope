 

HEADERS += \
    $$PWD/reader_thread.h \
    $$PWD/fourier_transform_thread.h \
    data_collector/scope_tia_client.h \
    data_collector/tia_client_impl/tia_client_version02.h

SOURCES += \
    $$PWD/reader_thread.cpp \
    $$PWD/fourier_transform_thread.cpp \
    data_collector/scope_tia_client.cpp \
    data_collector/tia_client_impl/tia_client_version02.cpp
