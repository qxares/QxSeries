#include "taskbarbrick.h"
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#include <string.h>

TaskbarBrick* taskbar_brick_new() {
    TaskbarBrick *brick = g_new(TaskbarBrick, 1);
    brick->window_ids = NULL;
    brick->window_names = NULL;
    return brick;
}

void taskbar_brick_free(TaskbarBrick *brick) {
    g_list_free_full(brick->window_ids, g_free);
    g_list_free_full(brick->window_names, g_free);
    g_free(brick);
}

void taskbar_brick_update_windows(TaskbarBrick *brick) {
    g_list_free_full(brick->window_ids, g_free);
    g_list_free_full(brick->window_names, g_free);
    brick->window_ids = NULL;
    brick->window_names = NULL;

    WnckHandle *handle = wnck_handle_new(WNCK_CLIENT_TYPE_APPLICATION);
    WnckScreen *screen = wnck_handle_get_default_screen(handle);
    wnck_screen_force_update(screen);
    GList *windows = wnck_screen_get_windows(screen);
    for (GList *l = windows; l; l = l->next) {
        WnckWindow *window = WNCK_WINDOW(l->data);
        if (wnck_window_is_skip_tasklist(window)) continue;
        const char *name = wnck_window_get_name(window);
        if (name && strcmp(name, "QxCentre Desktop") != 0 && strcmp(name, "QxCentre Taskbar") != 0) {
            char *id = g_strdup_printf("%lu", wnck_window_get_xid(window));
            brick->window_ids = g_list_append(brick->window_ids, id);
            brick->window_names = g_list_append(brick->window_names, g_strdup(name));
        }
    }
    g_object_unref(handle);
}

void taskbar_brick_raise_window(TaskbarBrick *brick, const gchar *window_id) {
    WnckHandle *handle = wnck_handle_new(WNCK_CLIENT_TYPE_APPLICATION);
    WnckScreen *screen = wnck_handle_get_default_screen(handle);
    GList *windows = wnck_screen_get_windows(screen);
    for (GList *l = windows; l; l = l->next) {
        WnckWindow *window = WNCK_WINDOW(l->data);
        char *id = g_strdup_printf("%lu", wnck_window_get_xid(window));
        if (strcmp(id, window_id) == 0) {
            wnck_window_activate(window, gtk_get_current_event_time());
            break;
        }
        g_free(id);
    }
    g_object_unref(handle);
}
