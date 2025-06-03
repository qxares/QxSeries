#include "mainwindowbrick.h"

MainwindowBrick* mainwindow_brick_new() {
    MainwindowBrick *brick = g_new0(MainwindowBrick, 1);
    brick->taskbar_brick = taskbar_brick_new();
    brick->gui_brick = taskbar_gui_brick_new();
    brick->handler_brick = taskbar_handler_brick_new(brick->taskbar_brick, brick->gui_brick);
    return brick;
}

void mainwindow_brick_free(MainwindowBrick *brick) {
    taskbar_handler_brick_free(brick->handler_brick);
    g_free(brick);
}
