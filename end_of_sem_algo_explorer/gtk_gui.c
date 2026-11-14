#include "gtk_gui.h"

// ---------------- Components ----------------

static GtkWidget *label(const char *text, const char *style) {
    GtkWidget *widget = gtk_label_new(text);
    gtk_label_set_xalign(GTK_LABEL(widget), 0);
    gtk_label_set_wrap(GTK_LABEL(widget), TRUE);
    if (style) gtk_widget_add_css_class(widget, style);
    return widget;
}

static GtkWidget *box(int spacing) {
    return gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
}

static void append(GtkWidget *parent, GtkWidget *child) {
    gtk_box_append(GTK_BOX(parent), child);
}

static void field(GtkWidget *parent, const char *title, GtkWidget *control) {
    append(parent, label(title, "heading"));
    append(parent, control);
}

static GtkWidget *card(const char *title, GtkWidget **value) {
    GtkWidget *widget = box(6);
    gtk_widget_add_css_class(widget, "card");
    gtk_widget_set_hexpand(widget, TRUE);
    append(widget, label(title, "dim-label"));
    *value = label("0", "title-2");
    append(widget, *value);
    return widget;
}

static GtkWidget *selector(const char *title, const char *const *options,
                           GtkWidget **control) {
    GtkWidget *component = box(8);
    *control = gtk_drop_down_new_from_strings(options);
    field(component, title, *control);
    return component;
}

static GtkWidget *data_input(ExplorerUI *ui) {
    GtkWidget *component = box(8);
    ui->input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ui->input), "64, 28, 85, 16, 47");
    field(component, "Input data", ui->input);
    append(component, label("Enter the data for your experiment.", "dim-label"));
    ui->apply = gtk_button_new_with_label("Apply data / Reset");
    append(component, ui->apply);
    return component;
}

static GtkWidget *playback(ExplorerUI *ui) {
    GtkWidget *component = box(12);
    ui->speed = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 100, 1200, 100);
    gtk_range_set_value(GTK_RANGE(ui->speed), 500);
    field(component, "Step delay (ms)", ui->speed);
    GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    ui->start = gtk_button_new_with_label("Start");
    ui->stop = gtk_button_new_with_label("Stop");
    ui->step = gtk_button_new_with_label("Step");
    gtk_widget_add_css_class(ui->start, "suggested-action");
    gtk_widget_set_hexpand(ui->start, TRUE);
    append(buttons, ui->start);
    append(buttons, ui->stop);
    append(buttons, ui->step);
    append(component, buttons);
    return component;
}

static GtkWidget *metrics(ExplorerUI *ui) {
    GtkWidget *component = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    append(component, card("INPUT SIZE · n", &ui->size));
    append(component, card("COMPARISONS", &ui->comparisons));
    append(component, card("OPERATIONS", &ui->operations));
    return component;
}

static GtkWidget *visualization(ExplorerUI *ui) {
    GtkWidget *component = gtk_overlay_new();
    ui->canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(ui->canvas, 400, 300);
    gtk_widget_set_vexpand(component, TRUE);
    gtk_widget_add_css_class(component, "visualization");
    gtk_overlay_set_child(GTK_OVERLAY(component), ui->canvas);
    ui->placeholder = label("Your visualization will appear here", "dim-label");
    gtk_widget_set_halign(ui->placeholder, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(ui->placeholder, GTK_ALIGN_CENTER);
    gtk_widget_set_can_target(ui->placeholder, FALSE);
    gtk_overlay_add_overlay(GTK_OVERLAY(component), ui->placeholder);
    return component;
}

static GtkWidget *workspace(ExplorerUI *ui) {
    GtkWidget *component = box(18);
    gtk_widget_add_css_class(component, "workspace");
    append(component, label("SIMULATION WORKSPACE", "dim-label"));
    append(component, label("Watch each operation unfold", "title-1"));
    append(component, visualization(ui));
    append(component, metrics(ui));
    ui->progress = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(ui->progress), TRUE);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(ui->progress), "Simulation progress");
    append(component, ui->progress);
    ui->status = label("Choose an algorithm and enter your data.", NULL);
    append(component, ui->status);
    return component;
}

static GtkWidget *panel(ExplorerUI *ui) {
    GtkWidget *component = box(14);
    gtk_widget_add_css_class(component, "controls");
    gtk_widget_set_size_request(component, 290, -1);
    append(component, label("EXPERIMENT SETUP", "dim-label"));
    append(component, label("Algorithm explorer", "title-2"));
    const char *structures[] = {"Array", "List", "Stack", "Queue", "Graph", "Tree", NULL};
    const char *algorithms[] = {"Select an algorithm", NULL};
    append(component, selector("Data structure", structures, &ui->structure));
    append(component, selector("Algorithm", algorithms, &ui->algorithm));
    ui->info = label("Select an algorithm to view its description and complexity.", NULL);
    gtk_widget_add_css_class(ui->info, "card");
    append(component, ui->info);
    append(component, data_input(ui));
    append(component, playback(ui));
    return component;
}

// Screen composition

GtkWidget *explorer_screen(ExplorerUI *ui) {
    GtkWidget *split = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_set_start_child(GTK_PANED(split), workspace(ui));
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), panel(ui));
    gtk_paned_set_end_child(GTK_PANED(split), scroll);
    gtk_paned_set_position(GTK_PANED(split), 800);
    gtk_paned_set_shrink_start_child(GTK_PANED(split), FALSE);
    gtk_paned_set_shrink_end_child(GTK_PANED(split), FALSE);
    gtk_paned_set_resize_end_child(GTK_PANED(split), FALSE);
    return split;
}

// Styles

static void apply_style(GtkWidget *window) {
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css,
        "window { background: #f0f0f0; color: #000000; }"
        ".visualization { background: #ffffff; border-radius: 10px; }"
        ".workspace { padding: 26px; } .controls { padding: 24px; background: #f0f0f0; }"
        ".card { background: #ffffff; border-radius: 10px; padding: 16px; }"
        ".dim-label { color: #0000ff; } .title-1 { font-size: 25px; font-weight: bold; }"
        ".title-2 { font-size: 21px; font-weight: bold; }"
        "button, entry, dropdown { border-radius: 7px; }"
        "progressbar progress { background: #3ba6d4; }");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(window),
        GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);
}

void activate(GtkApplication *app, gpointer data) {
    (void)data;
    ExplorerUI *ui = g_new0(ExplorerUI, 1);
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Group 4 · Algorithm Explorer");
    gtk_window_set_default_size(GTK_WINDOW(window), 1180, 780);
    apply_style(window);
    gtk_window_set_child(GTK_WINDOW(window), explorer_screen(ui));
    g_object_set_data_full(G_OBJECT(window), "explorer", ui, g_free);
    gtk_window_present(GTK_WINDOW(window));
}
