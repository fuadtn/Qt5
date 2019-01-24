QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phonebook
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    phonebook.cpp

HEADERS  += mainwindow.h \
    phonebook.h

FORMS    += mainwindow.ui

CONFIG += release

RESOURCES += \
    resource.qrc

VERSION = 1.4.0.0
QMAKE_TARGET_COMPANY = K&K
QMAKE_TARGET_PRODUCT = Phonebook
QMAKE_TARGET_DESCRIPTION = K&K Phonebook
RC_ICONS += pbook.ico

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/ -lQtXlsx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/ -lQtXlsxd
else:unix: LIBS += -L$$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/ -lQtXlsx

INCLUDEPATH += $$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/include/QtXlsx
DEPENDPATH += $$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/include/QtXlsx

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/libQtXlsx.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/libQtXlsxd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/QtXlsx.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/QtXlsxd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../Qt/5.5/msvc2010s_ax_sqlite/lib/libQtXlsx.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Qt/lib/ -lQRestAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Qt/lib/ -lQRestAPId
else:unix: LIBS += -L$$PWD/../../Qt/lib/ -lQRestAPI

INCLUDEPATH += $$PWD/../../Qt/lib/include
DEPENDPATH += $$PWD/../../Qt/lib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/lib/libQRestAPI.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/lib/libQRestAPId.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/lib/QRestAPI.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Qt/lib/QRestAPId.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../Qt/lib/libQRestAPI.a
