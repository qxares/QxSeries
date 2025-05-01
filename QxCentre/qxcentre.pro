QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    basedialogbrick.cpp \
    infodialogbrick.cpp \
    interlinkbrick.cpp \
    interlinkmaestrobrick.cpp \
    main.cpp \
    mainwindowbrick.cpp \
    startupbrick.cpp \
    themebrick.cpp \
    windowbrick.cpp
HEADERS += \
    basedialogbrick.h \
    infodialogbrick.h \
    interlinkbrick.h \
    interlinkmaestrobrick.h \
    mainwindowbrick.h \
    startupbrick.h \
    themebrick.h \
    windowbrick.h
QMAKE_CXXFLAGS += -std=c++11
