QT       += core gui multimedia widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindowbrick.cpp \
    themebrick.cpp \
    interlinkbrick.cpp \
    ../QxText/QxWrite/qxwritewindowbrick.cpp \
    ../QxText/QxSheet/qxsheetwindowbrick.cpp \
    ../QxAudio/QxAudioPlayer/qxmusicplayerwindow.cpp \
    ../QxAudio/QxAudioPlayer/visualizerwidget.cpp

HEADERS += \
    mainwindowbrick.h \
    themebrick.h \
    interlinkbrick.h \
    ../QxText/QxWrite/qxwritewindowbrick.h \
    ../QxText/QxSheet/qxsheetwindowbrick.h \
    ../QxAudio/QxAudioPlayer/qxmusicplayerwindow.h \
    ../QxAudio/QxAudioPlayer/visualizerwidget.h

# Link TagLib for album art
PKGCONFIG += taglib
LIBS += -ltag

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
