#include "gtk_gui.h"

const char *const algorithm_info[] = {
    "Compares adjacent values and swaps them when they are out of order.\n"
    "Time: O(n²) for this implementation. Extra space: O(1).",

    "Builds a sorted prefix by shifting larger values to make room for each new value.\n"
    "Time: O(n) best, O(n²) average and worst. Extra space: O(1).",

    "Partitions values around a pivot, then sorts each partition.\n"
    "Time: O(n log n) average, O(n²) worst.",

    "Finds shortest paths from a starting vertex in a graph with nonnegative edge weights.\n"
    "Time: O((V + E) log V) with an adjacency list and a binary heap.",

    "Finds a route of minimum total weight between vertices.\n"
    "Complexity depends on the algorithm used."
};

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
    gtk_entry_set_placeholder_text(GTK_ENTRY(ui->input), "64, 28, 85, 16, 47, 103, 28, 55");
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

static void on_algorithm_changed(GtkDropDown *dropdown, GParamSpec *pspec,
                                 gpointer data)
{
    ExplorerUI *ui = data;
    guint index = gtk_drop_down_get_selected(dropdown);

    if (index < G_N_ELEMENTS(algorithm_info))
        gtk_label_set_text(GTK_LABEL(ui->info), algorithm_info[index]);
}

static void update_metrics(ExplorerUI *ui)
{
    char text[32];

    g_snprintf(text, sizeof text, "%" G_GUINT64_FORMAT, ui->run.operations);
    gtk_label_set_text(GTK_LABEL(ui->operations), text);

    g_snprintf(text, sizeof text, "%" G_GUINT64_FORMAT, ui->run.comparisons);
    gtk_label_set_text(GTK_LABEL(ui->comparisons), text);
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
    const char *algorithms[] = {"Bubble Sort", "Insertion Sort", "Quick Sort", "Djikstra's Algorithm", "Shortest Path", NULL};
    append(component, selector("Data structure", structures, &ui->structure));
    append(component, selector("Algorithm", algorithms, &ui->algorithm));

    ui->info = label(algorithm_info[0], NULL);
    gtk_widget_add_css_class(ui->info, "card");
    append(component, ui->info);

    g_signal_connect(ui->algorithm, "notify::selected",
                     G_CALLBACK(on_algorithm_changed), ui);
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

static void draw_canvas(GtkDrawingArea *area, cairo_t *cr,
                        int width, int height, gpointer data)
{
    (void)area;
    ExplorerUI *ui = data;

    if (ui->draw)
        ui->draw(cr, width, height, ui->run.state);
}

static void on_step_clicked(GtkButton *button, gpointer data)
{
    ExplorerUI *ui = data;

    if (!ui->run.advance || ui->run.done)
        return;

    ui->run.advance(&ui->run);
    update_metrics(ui);

    gtk_widget_queue_draw(ui->canvas);
    gtk_widget_set_sensitive(GTK_WIDGET(button), !ui->run.done);
}

static void set_run(ExplorerUI *ui, Step run, DrawAlgorithm draw)
{
    ui->run = run;
    ui->draw = draw;

    gtk_widget_set_visible(ui->placeholder, FALSE);
    gtk_widget_set_sensitive(ui->step, !run.done);
    gtk_widget_queue_draw(ui->canvas);
}

void draw_bubble(cairo_t *cr, int width, int height, const void *state)
{
    const BubbleState *s = state;
    if (s->count == 0 || width <= 24 || height <= 40) return;

    // Find the range. Including zero also supports negative values.
    double min = 0, max = 0;
    for (int i = 0; i < s->count; i++) {
        min = MIN(min, s->values[i]);
        max = MAX(max, s->values[i]);
    }

    double chart_height = height - 40.0;  // Leave room for labels.
    double scale = (max > min) ? chart_height / (max - min) : 0;
    double baseline = 12 + max * scale;
    double slot = (width - 24.0) / s->count;

    cairo_set_font_size(cr, 14);

    for (int i = 0; i < s->count; i++) {
        double center = 12 + (i + 0.5) * slot;
        double tip = baseline - s->values[i] * scale;
        double bar_width = slot * 0.7;

        // Draw the bar between zero and its value.
        cairo_set_source_rgb(cr, 0.20, 0.55, 0.85);
        cairo_rectangle(cr, center - bar_width / 2, MIN(tip, baseline),
                        bar_width, MAX(tip, baseline) - MIN(tip, baseline));
        if (s->compared >= 0 && (i == s->compared || i == s->compared + 1)) {
            cairo_fill_preserve(cr);                // Fill blue and keep the rectangle.
            cairo_set_source_rgb(cr, 0.75, 0.25, 0.05);
            cairo_set_line_width(cr, 3);
            cairo_stroke(cr);                       // Outline the same rectangle.
        } else {
            cairo_fill(cr);
        }

        // Center the value beneath the bar.
        char text[16];
        g_snprintf(text, sizeof text, "%d", s->values[i]);

        cairo_text_extents_t ext;
        cairo_text_extents(cr, text, &ext);
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_move_to(cr, center - ext.width / 2 - ext.x_bearing, height - 8);
        cairo_show_text(cr, text);
    }
}

static void on_apply_clicked(GtkButton *button, gpointer data)
{
    ExplorerUI *ui = data;
    const char *text = gtk_editable_get_text(GTK_EDITABLE(ui->input));

    // Use the displayed default if the user entered only spaces or nothing.
    while (g_ascii_isspace(*text)) text++;
    if (*text == '\0')
        text = gtk_entry_get_placeholder_text(GTK_ENTRY(ui->input));

    gchar **parts = g_strsplit(text, ",", -1);
    int count = (int)g_strv_length(parts);
    int *values = g_new(int, count);

    for (int i = 0; i < count; i++) {
        gint64 value;
        if (!g_ascii_string_to_signed(g_strstrip(parts[i]), 10,
                                      G_MININT, G_MAXINT, &value, NULL)) {
            gtk_label_set_text(GTK_LABEL(ui->status),
                               "Enter integers separated by commas, e.g. 5, 2, 4.");
            g_strfreev(parts);
            g_free(values);
            return;
                                      }
        values[i] = (int)value;
    }
    g_strfreev(parts);

    // Keep the array alive for drawing and stepping. Apply replaces the old one.
    g_object_set_data_full(G_OBJECT(button), "values", values, g_free);
    g_clear_handle_id(&ui->timer_id, g_source_remove);

    ui->bubble = (BubbleState){
        .values = values, .count = count, .end = count - 1, .compared = -1
    };
    ui->run = (Step){
        .state = &ui->bubble,
        .advance = n_bubble_step,
        .done = count < 2
    };
    update_metrics(ui);
    ui->draw = draw_bubble;

    gtk_label_set_text(GTK_LABEL(ui->status), "Data loaded.");
    gtk_widget_set_visible(ui->placeholder, FALSE);
    gtk_widget_set_sensitive(ui->step, !ui->run.done);
    gtk_widget_queue_draw(ui->canvas);
}

static gboolean autoplay_tick(gpointer data)
{
    ExplorerUI *ui = data;

    // Same comparison, metrics update, and redraw as a manual click.
    on_step_clicked(GTK_BUTTON(ui->step), ui);

    if (ui->run.done) {
        ui->timer_id = 0;
        return G_SOURCE_REMOVE;
    }
    return G_SOURCE_CONTINUE;
}

static void on_start_clicked(GtkButton *button, gpointer data)
{
    ExplorerUI *ui = data;
    if (ui->timer_id || !ui->run.advance || ui->run.done) return;

    guint delay = (guint)gtk_range_get_value(GTK_RANGE(ui->speed));
    ui->timer_id = g_timeout_add(delay, autoplay_tick, ui);
}

static void on_stop_clicked(GtkButton *button, gpointer data)
{
    ExplorerUI *ui = data;
    g_clear_handle_id(&ui->timer_id, g_source_remove);
}

static void free_explorer(gpointer data)
{
    ExplorerUI *ui = data;
    g_clear_handle_id(&ui->timer_id, g_source_remove);
    g_free(ui);
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
    g_object_set_data_full(G_OBJECT(window), "explorer", ui, free_explorer);

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ui->canvas),
                                   draw_canvas, ui, NULL);
    g_signal_connect(ui->apply, "clicked", G_CALLBACK(on_apply_clicked), ui);
    g_signal_connect(ui->step, "clicked", G_CALLBACK(on_step_clicked), ui);
    g_signal_connect(ui->start, "clicked", G_CALLBACK(on_start_clicked), ui);
    g_signal_connect(ui->stop,  "clicked", G_CALLBACK(on_stop_clicked), ui);

    gtk_widget_set_sensitive(ui->step, FALSE);

    gtk_window_present(GTK_WINDOW(window));
}
