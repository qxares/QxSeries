#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include "taskbarbrick.h"
#include "taskbarguibrick.h"
#include "taskbarhandlerbrick.h"

typedef struct {
    TaskbarBrick *taskbar_brick;
    TaskbarGuiBrick *gui_brick;
    TaskbarHandlerBrick *handler_brick;
} MainwindowBrick;

MainwindowBrick* mainwindow_brick_new();
void mainwindow_brick_free(MainwindowBrick *brick);

#endif // MAINWINDOWBRICK_H
