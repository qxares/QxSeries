#-------------------------------------------------
#
# Project created by QtCreator 2025-05-03
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QxCentre
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindowbrick.cpp \
    themebrick.cpp \
    interlinkbrick.cpp \
    windowbrick.cpp \
    startupbrick.cpp

HEADERS += \
    mainwindowbrick.h \
    themebrick.h \
    interlinkbrick.h \
    windowbrick.h \
    startupbrick.h

QMAKE_CXXFLAGS += -std=c++11
