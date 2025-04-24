QT       += core gui widgets

TARGET = QxCentre
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindowbrick.cpp \
    themebrick.cpp \
    windowmanagerbrick.cpp \
    ../QxText/QxWrite/qxwritewindowbrick.cpp \
    ../QxText/QxSheet/qxsheetwindowbrick.cpp \
    ../QxAudio/QxAudioPlayer/qxmusicplayerwindow.cpp

HEADERS += \
    mainwindowbrick.h \
    themebrick.h \
    windowmanagerbrick.h \
    ../QxText/QxWrite/qxwritewindowbrick.h \
    ../QxText/QxSheet/qxsheetwindowbrick.h \
    ../QxAudio/QxAudioPlayer/qxmusicplayerwindow.h
