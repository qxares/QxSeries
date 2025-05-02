QT       += core gui widgets

TARGET = QxCentre
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    basedialogbrick.cpp \
    infodialogbrick.cpp \
    interlinkbrick.cpp \
    interlinkmaestrobrick.cpp \
    main.cpp \
    mainwindowbrick.cpp \
    startupbrick.cpp \
    themebrick.cpp \
    windowbrick.cpp \
    desktopmaestrobrick.cpp

HEADERS += \
    basedialogbrick.h \
    infodialogbrick.h \
    interlinkbrick.h \
    interlinkmaestrobrick.h \
    mainwindowbrick.h \
    startupbrick.h \
    themebrick.h \
    windowbrick.h \
    desktopmaestrobrick.h

QMAKE_CXXFLAGS += -std=c++11
