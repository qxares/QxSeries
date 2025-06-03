#ifndef TASKBARGUIBRICK_H
#define TASKBARGUIBRICK_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *taskbar;
    GtkWidget *start_button;
    GtkWidget *window_box;
    GtkWidget *time_label;
    GHashTable *window_buttons; // Maps window IDs to buttons
    void (*window_clicked_callback)(const gchar *window_id, gpointer data);
    gpointer callback_data;
} TaskbarGuiBrick;

TaskbarGuiBrick* taskbar_gui_brick_new();
void taskbar_gui_brick_free(TaskbarGuiBrick *brick);
void taskbar_gui_brick_update_windows(TaskbarGuiBrick *brick, GList *window_ids, GList *window_names);
void taskbar_gui_brick_set_window_clicked_callback(TaskbarGuiBrick *brick, void (*callback)(const gchar*, gpointer), gpointer data);

#endif // TASKBARGUIBRICK_H
