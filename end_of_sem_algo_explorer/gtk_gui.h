#pragma once
#include <gtk/gtk.h>
#include "algorithms.h"

typedef void (*DrawAlgorithm)(cairo_t *cr, int width, int height,
                              const void *state);

typedef struct {
    GtkWidget *canvas, *placeholder;
    GtkWidget *input, *target, *target_field, *structure, *algorithm, *speed;
    GtkWidget *status, *comparisons, *operations, *size, *progress, *info;
    GtkWidget *apply, *start, *stop, *step;  // Playback buttons
    guint timer_id;
    Step run;                              // Active algorithm and its state
    BubbleState bubble;
    BinarySearchState binary_search;
    DrawAlgorithm draw;
} ExplorerUI;

GtkWidget *explorer_screen(ExplorerUI *ui);
void activate(GtkApplication *app, gpointer data);
