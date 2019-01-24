QT += core network
QT -= gui

CONFIG += c++11

TARGET = httpServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    famazons3.cpp \
    faudio.cpp \
    faudiowav.cpp \
    fhttpserver.cpp \
    fhttpurls.cpp \
    fhttpcontrollers.cpp


DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    famazons3.h \
    faudio.h \
    faudiowav.h \
    fhttpserver.h \
    fhttpurls.h \
    fhttpcontrollers.h

#INCLUDEPATH += aws/include
#DEPENDPATH += aws/include
#unix:!macx: LIBS += aws/lib/libaws-cpp-sdk-core.so
#unix:!macx: LIBS += aws/lib/libaws-cpp-sdk-s3.so




unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/linux/intel64/Release/ -laws-cpp-sdk-s3

INCLUDEPATH += $$PWD/../../../../usr/local/include/aws
DEPENDPATH += $$PWD/../../../../usr/local/include/aws

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/linux/intel64/Release/libaws-cpp-sdk-s3.a

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/linux/intel64/Release/ -laws-cpp-sdk-core

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/linux/intel64/Release/libaws-cpp-sdk-core.a


unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/linux/intel64/Release/ -ltesting-resources


unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/linux/intel64/Release/libtesting-resources.a


unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/linux/intel64/Release/ -laws-cpp-sdk-s3-encryption

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/linux/intel64/Release/libaws-cpp-sdk-s3-encryption.a
