#ifndef TASKBARBRICK_H
#define TASKBARBRICK_H

#include <glib.h>

typedef struct {
    GList *window_ids; // List of window IDs
    GList *window_names; // List of window names
} TaskbarBrick;

TaskbarBrick* taskbar_brick_new();
void taskbar_brick_free(TaskbarBrick *brick);
void taskbar_brick_update_windows(TaskbarBrick *brick);
void taskbar_brick_raise_window(TaskbarBrick *brick, const gchar *window_id);

#endif // TASKBARBRICK_H
