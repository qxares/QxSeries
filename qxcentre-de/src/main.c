#include <gtk/gtk.h>
#include "bricks/mainwindowbrick.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    MainwindowBrick *mainwindow = mainwindow_brick_new();
    gtk_main();
    mainwindow_brick_free(mainwindow);
    return 0;
}
