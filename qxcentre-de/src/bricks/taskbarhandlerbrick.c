#include "taskbarhandlerbrick.h"

static void on_window_clicked(const gchar *window_id, gpointer data) {
    TaskbarHandlerBrick *brick = (TaskbarHandlerBrick*)data;
    taskbar_brick_raise_window(brick->taskbar_brick, window_id);
}

static gboolean update_windows(gpointer data) {
    TaskbarHandlerBrick *brick = (TaskbarHandlerBrick*)data;
    taskbar_brick_update_windows(brick->taskbar_brick);
    taskbar_gui_brick_update_windows(brick->gui_brick, brick->taskbar_brick->window_ids, brick->taskbar_brick->window_names);
    return TRUE;
}

TaskbarHandlerBrick* taskbar_handler_brick_new(TaskbarBrick *taskbar_brick, TaskbarGuiBrick *gui_brick) {
    TaskbarHandlerBrick *brick = g_new0(TaskbarHandlerBrick, 1);
    brick->taskbar_brick = taskbar_brick;
    brick->gui_brick = gui_brick;

    taskbar_gui_brick_set_window_clicked_callback(gui_brick, on_window_clicked, brick);

    brick->update_source = g_timeout_source_new_seconds(1);
    g_source_set_callback(brick->update_source, (GSourceFunc)update_windows, brick, NULL);
    g_source_attach(brick->update_source, NULL);

    update_windows(brick);
    return brick;
}

void taskbar_handler_brick_free(TaskbarHandlerBrick *brick) {
    g_source_destroy(brick->update_source);
    taskbar_brick_free(brick->taskbar_brick);
    taskbar_gui_brick_free(brick->gui_brick);
    g_free(brick);
}
