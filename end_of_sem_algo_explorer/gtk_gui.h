#pragma once
#include <gtk/gtk.h>

// Widget references for a controller to connect signals and render results.
// Widgets belong to the screen. Keep this struct alive while using the screen.
typedef struct {
    GtkWidget *canvas, *placeholder;
    GtkWidget *input, *structure, *algorithm, *speed;
    GtkWidget *status, *comparisons, *operations, *size, *progress, *info;
    GtkWidget *apply, *start, *stop, *step;
} ExplorerUI;

// Builds the view only; no parsing, simulation, or algorithm callbacks.
// A renderer can set canvas's draw function and hide placeholder when ready.
GtkWidget *explorer_screen(ExplorerUI *ui);
void activate(GtkApplication *app, gpointer data);
