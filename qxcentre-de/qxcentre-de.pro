QT += core gui widgets
TARGET = QxCentreDE
TEMPLATE = app
SOURCES += src/main.cpp \
           src/bricks/taskbarbrick.cpp \
           src/bricks/taskbarguibrick.cpp \
           src/bricks/taskbarhandlerbrick.cpp \
           src/bricks/mainwindowbrick.cpp
HEADERS += src/bricks/taskbarbrick.h \
           src/bricks/taskbarguibrick.h \
           src/bricks/taskbarhandlerbrick.h \
           src/bricks/mainwindowbrick.h
