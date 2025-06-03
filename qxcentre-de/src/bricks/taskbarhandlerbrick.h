#ifndef TASKBARHANDLERBRICK_H
#define TASKBARHANDLERBRICK_H

#include "taskbarbrick.h"
#include "taskbarguibrick.h"

typedef struct {
    TaskbarBrick *taskbar_brick;
    TaskbarGuiBrick *gui_brick;
    GSource *update_source;
} TaskbarHandlerBrick;

TaskbarHandlerBrick* taskbar_handler_brick_new(TaskbarBrick *taskbar_brick, TaskbarGuiBrick *gui_brick);
void taskbar_handler_brick_free(TaskbarHandlerBrick *brick);

#endif // TASKBARHANDLERBRICK_H
