QT       += network
QT       -= gui

TARGET = QRestAPI
TEMPLATE = lib
CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        qrestapi.cpp

HEADERS += \
        qrestapi.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
