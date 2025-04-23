QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QxCentre
TEMPLATE = app

SOURCES += main.cpp \
           mainwindowbrick.cpp \
           themebrick.cpp \
           ../QxText/QxWrite/qxwritewindowbrick.cpp \
           ../QxText/QxSheet/qxsheetwindowbrick.cpp

HEADERS += mainwindowbrick.h \
           themebrick.h \
           ../QxText/QxWrite/qxwritewindowbrick.h \
           ../QxText/QxSheet/qxsheetwindowbrick.h

INCLUDEPATH += ../QxText/QxWrite \
               ../QxText/QxSheet
