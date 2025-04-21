QT += core gui widgets

CONFIG += c++11

TARGET = QxCentre

TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindowbrick.cpp \
    ../QxWrite/qxwritewindowbrick.cpp \
    ../QxSheet/qxsheetwindowbrick.cpp \
    themebrick.cpp

HEADERS += \
    mainwindowbrick.h \
    ../QxWrite/qxwritewindowbrick.h \
    ../QxSheet/qxsheetwindowbrick.h \
    themebrick.h

INCLUDEPATH += ../QxWrite ../QxSheet

QMAKE_CXXFLAGS += -Wall -Wextra
