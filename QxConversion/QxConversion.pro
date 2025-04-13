QT += core gui widgets
CONFIG += console
CONFIG -= app_bundle

TARGET = QxConversion
TEMPLATE = app

SOURCES += \
    main.cpp \
    qxconversionbrick.cpp \
    cleanhtmlbrick.cpp \
    dialogbrick.cpp

HEADERS += \
    qxconversionbrick.h \
    cleanhtmlbrick.h \
    dialogbrick.h

DESTDIR = .

QMAKE_POST_LINK += chmod +x $$DESTDIR/QxConversion
