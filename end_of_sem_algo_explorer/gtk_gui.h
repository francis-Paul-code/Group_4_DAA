#pragma once
#include <gtk/gtk.h>
#include "algorithms.h"
#include "datastructures.h"

typedef void (*DrawAlgorithm)(cairo_t *cr, int width, int height,
                              const void *state);

typedef struct {
    GtkWidget *canvas, *placeholder;
    GtkWidget *input, *structure, *algorithm, *speed;
    GtkWidget *status, *comparisons, *operations, *size, *progress, *info;
    GtkWidget *apply, *start, *stop, *step;  // Playback buttons
    Any input_data;
    guint timer_id;
    Step run;                              // Active algorithm and its state
    BubbleState bubble;
    DrawAlgorithm draw;
    const void *draw_data;
} ExplorerUI;

GtkWidget *explorer_screen(ExplorerUI *ui);
void activate(GtkApplication *app, gpointer data);
