#include "taskbarguibrick.h"
#include <time.h>

static void on_start_menu_item_activate(GtkMenuItem *item, gpointer data) {
    const char *cmd = (const char*)data;
    g_spawn_command_line_async(cmd, NULL);
}

static void update_time_label(gpointer data) {
    TaskbarGuiBrick *brick = (TaskbarGuiBrick*)data;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%H:%M %d/%m/%Y", tm);
    gtk_label_set_text(GTK_LABEL(brick->time_label), time_str);
}

static void on_window_button_clicked(GtkButton *button, gpointer data) {
    const gchar *window_id = (const gchar*)g_object_get_data(G_OBJECT(button), "window-id");
    TaskbarGuiBrick *brick = (TaskbarGuiBrick*)g_object_get_data(G_OBJECT(button), "brick");
    if (brick->window_clicked_callback) {
        brick->window_clicked_callback(window_id, brick->callback_data);
    }
}

static void on_start_button_clicked(GtkButton *button, gpointer data) {
    TaskbarGuiBrick *brick = (TaskbarGuiBrick*)data;
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *dev_menu = gtk_menu_new();
    GtkWidget *dev_item = gtk_menu_item_new_with_label("Development");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(dev_item), dev_menu);

    GtkWidget *term_item = gtk_menu_item_new_with_label("Terminal");
    g_signal_connect(term_item, "activate", G_CALLBACK(on_start_menu_item_activate), (gpointer)"lxterminal");
    gtk_menu_shell_append(GTK_MENU_SHELL(dev_menu), term_item);

    GtkWidget *file_item = gtk_menu_item_new_with_label("Filemanager");
    g_signal_connect(file_item, "activate", G_CALLBACK(on_start_menu_item_activate), (gpointer)"pcmanfm");
    gtk_menu_shell_append(GTK_MENU_SHELL(dev_menu), file_item);

    GtkWidget *note_item = gtk_menu_item_new_with_label("Notepad");
    g_signal_connect(note_item, "activate", G_CALLBACK(on_start_menu_item_activate), (gpointer)"mousepad");
    gtk_menu_shell_append(GTK_MENU_SHELL(dev_menu), note_item);

    GtkWidget *firefox_item = gtk_menu_item_new_with_label("Firefox");
    g_signal_connect(firefox_item, "activate", G_CALLBACK(on_start_menu_item_activate), (gpointer)"firefox-esr");
    gtk_menu_shell_append(GTK_MENU_SHELL(dev_menu), firefox_item);

    GtkWidget *exit_item = gtk_menu_item_new_with_label("Exit");
    g_signal_connect(exit_item, "activate", G_CALLBACK(on_start_menu_item_activate), (gpointer)"openbox --exit");
    gtk_menu_shell_append(GTK_MENU_SHELL(dev_menu), exit_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), dev_item);
    gtk_widget_show_all(menu);
    gtk_menu_popup_at_widget(GTK_MENU(menu), brick->start_button, GDK_GRAVITY_NORTH_WEST, GDK_GRAVITY_SOUTH_WEST, NULL);
}

TaskbarGuiBrick* taskbar_gui_brick_new() {
    TaskbarGuiBrick *brick = g_new0(TaskbarGuiBrick, 1);
    brick->window_buttons = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    // Create a top-level window for the taskbar
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "QxCentre Taskbar");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // Remove window decorations
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE); // Keep above other windows
    gtk_window_move(GTK_WINDOW(window), 0, 0); // Position at top-left
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 40); // Example size (adjust as needed)

    brick->taskbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    brick->start_button = gtk_button_new_with_label("QxCentre");
    brick->window_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    brick->time_label = gtk_label_new("");

    gtk_container_add(GTK_CONTAINER(window), brick->taskbar); // Add taskbar to window
    gtk_box_pack_start(GTK_BOX(brick->taskbar), brick->start_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(brick->taskbar), brick->window_box, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(brick->taskbar), brick->time_label, FALSE, FALSE, 0);

    g_signal_connect(brick->start_button, "clicked", G_CALLBACK(on_start_button_clicked), brick);

    GSource *source = g_timeout_source_new_seconds(1);
    g_source_set_callback(source, (GSourceFunc)update_time_label, brick, NULL);
    g_source_attach(source, NULL);

    update_time_label(brick);
    gtk_widget_show_all(window); // Show the window
    return brick;
}

void taskbar_gui_brick_free(TaskbarGuiBrick *brick) {
    g_hash_table_destroy(brick->window_buttons);
    gtk_widget_destroy(brick->taskbar); // Destroy the taskbar widget
    g_free(brick);
}

void taskbar_gui_brick_update_windows(TaskbarGuiBrick *brick, GList *window_ids, GList *window_names) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(brick->window_box));
    for (GList *l = children; l; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }
    g_list_free(children);
    g_hash_table_remove_all(brick->window_buttons);

    GList *id_iter = window_ids;
    GList *name_iter = window_names;
    while (id_iter && name_iter) {
        const gchar *id = (const gchar*)id_iter->data;
        const gchar *name = (const gchar*)name_iter->data;
        GtkWidget *button = gtk_button_new_with_label(name);
        g_object_set_data_full(G_OBJECT(button), "window-id", g_strdup(id), g_free);
        g_object_set_data(G_OBJECT(button), "brick", brick);
        g_signal_connect(button, "clicked", G_CALLBACK(on_window_button_clicked), NULL);
        gtk_box_pack_start(GTK_BOX(brick->window_box), button, FALSE, FALSE, 0);
        g_hash_table_insert(brick->window_buttons, g_strdup(id), button);
        id_iter = id_iter->next;
        name_iter = name_iter->next;
    }
    gtk_widget_show_all(brick->window_box);
}

void taskbar_gui_brick_set_window_clicked_callback(TaskbarGuiBrick *brick, void (*callback)(const gchar*, gpointer), gpointer data) {
    brick->window_clicked_callback = callback;
    brick->callback_data = data;
}
