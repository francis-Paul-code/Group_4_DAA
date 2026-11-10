#include <gtk/gtk.h>
#include <stdio.h>
#include "algorithms.h"

const struct Algorithm algorithms[] = {
    {
        .name = "Binary Search",
        .action = &binary_search,
    },
{
    .name = "Insertion Sort",
    .action = &insertion_sort,
},
{
    .name = "Merge Sort",
    .action = &merge_sort,
},
{
    .name = "Bubble Sort",
    .action = &bubble_sort,
}
};

static void _workspace(GtkWidget *workspace) {

    GtkWidget *canvas = gtk_drawing_area_new();
    gtk_widget_set_vexpand(canvas, TRUE);
    gtk_box_append(GTK_BOX(workspace), canvas);
}

static void _panel(GtkWidget *panel) {
    GtkWidget *heading = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(heading), "<b>Algorithm information</b>");
    gtk_label_set_xalign(GTK_LABEL(heading), 0.0f);
    gtk_box_append(GTK_BOX(panel), heading);

    GtkWidget *divider = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(panel), divider);
    GtkWidget *algo_header = gtk_label_new("Select Algorithm");
    gtk_label_set_xalign(GTK_LABEL(algo_header), 2.0f);
    gtk_box_append(GTK_BOX(panel), algo_header);
    gtk_box_append(GTK_BOX(panel), divider);


    GtkWidget *description = gtk_label_new("No algorithm selected.");
    gtk_label_set_xalign(GTK_LABEL(description), 0.0f);
    gtk_label_set_wrap(GTK_LABEL(description), TRUE);
    gtk_box_append(GTK_BOX(panel), description);

}
static void activate(GtkApplication *app, gpointer data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Group 4 Algorithm Explorer");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);

    // split pane into two horizontally
    GtkWidget *split = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_window_set_child(GTK_WINDOW(window), split);

    // left: visualization workspace
    GtkWidget *workspace = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_paned_set_start_child(GTK_PANED(split), workspace);

   _workspace(workspace);

    // right: information panel
    GtkWidget *panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(panel, 16);
    gtk_widget_set_margin_bottom(panel, 16);
    gtk_widget_set_margin_start(panel, 16);
    gtk_widget_set_margin_end(panel, 16);
    gtk_paned_set_end_child(GTK_PANED(split), panel);

    // panel interface
    _panel(panel);

    // Start with roughly 700 px for the canvas and 300 px for the panel.
    gtk_paned_set_position(GTK_PANED(split), 700);
    gtk_paned_set_resize_start_child(GTK_PANED(split), TRUE);
    gtk_paned_set_resize_end_child(GTK_PANED(split), FALSE);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("com.daa.algorithmexplorer", 0);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}