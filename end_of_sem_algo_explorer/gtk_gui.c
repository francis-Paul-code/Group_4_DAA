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

    "Finds the smallest unsorted value and swaps it to the front, one pass at a time.\n"
    "Time: O(n²) in every case. Extra space: O(1).",

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
                                 gpointer data) {
    ExplorerUI *ui = data;
    guint index = gtk_drop_down_get_selected(dropdown);

    if (index < G_N_ELEMENTS(algorithm_info))
        gtk_label_set_text(GTK_LABEL(ui->info), algorithm_info[index]);
}

static void update_metrics(ExplorerUI *ui) {
    char text[32];

    g_snprintf(text, sizeof text, "%" G_GUINT64_FORMAT, ui->run.operations);
    gtk_label_set_text(GTK_LABEL(ui->operations), text);

    g_snprintf(text, sizeof text, "%" G_GUINT64_FORMAT, ui->run.comparisons);
    gtk_label_set_text(GTK_LABEL(ui->comparisons), text);
}

static void show_structure(ExplorerUI *ui,
                           DrawAlgorithm draw,
                           const void *structure) {
    g_clear_handle_id(&ui->timer_id, g_source_remove);

    ui->draw = draw;
    ui->draw_data = structure;

    gtk_widget_set_sensitive(ui->start, FALSE);
    gtk_widget_set_sensitive(ui->stop, FALSE);
    gtk_widget_set_sensitive(ui->step, FALSE);
    gtk_widget_set_sensitive(ui->speed, FALSE);

    gtk_widget_set_visible(ui->placeholder, FALSE);
    gtk_widget_queue_draw(ui->canvas);
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
    const char *algorithms[] = {"Bubble Sort", "Insertion Sort", "Quick Sort", "Djikstra's Algorithm", "Selection Sort", NULL};
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

//Stacks
void draw_stack(cairo_t *cr, int width, int height, const void *data)
{
    const Stack *stack = data;
    List *list = stack->base;


    if (!list || list->size == 0)
        return;

    double box_width = MIN(160.0, width * 0.5);
    double box_height = MIN(45.0, (height - 60.0) / list->size);

    if (box_height <= 0)
        return;

    double x = (width - box_width) / 2.0;
    double top = (height - box_height * list->size) / 2.0;

    cairo_set_font_size(cr, MIN(18.0, box_height * 0.5));

    for (int i = 0; i < list->size; i++) {
        int value = *(int *)get(list,i);
        double y = top + (list->size - 1 - i) * box_height;

        cairo_rectangle(cr, x, y, box_width, box_height);
        cairo_set_source_rgb(cr, 0.20, 0.55, 0.85);
        cairo_fill_preserve(cr);

        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_set_line_width(cr, 1);
        cairo_stroke(cr);

        char text[32];
        g_snprintf(text, sizeof text, "%d", value);

        cairo_text_extents_t ext;
        cairo_text_extents(cr, text, &ext);
        cairo_move_to(cr,
            x + (box_width - ext.width) / 2 - ext.x_bearing,
            y + (box_height - ext.height) / 2 - ext.y_bearing);
        cairo_show_text(cr, text);
    }

    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, x, top - 10);
    cairo_show_text(cr, "TOP");
}

static void setup_stack(ExplorerUI *ui)
{
    if (!ui->input_data)
        return;
    show_structure(ui, draw_stack, ui->input_data);
}

// Graphs

// Bubble Sort
void draw_bubble(cairo_t *cr, int width, int height, const void *state) {
    const BubbleState *s = state;
    if (s->count == 0 || width <= 24 || height <= 40) return;

    // Find the range. Including zero also supports negative values.
    double min = 0, max = 0;
    for (int i = 0; i < s->count; i++) {
        min = MIN(min, s->values[i]);
        max = MAX(max, s->values[i]);
    }

    double chart_height = height - 40.0; // Leave room for labels.
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
            cairo_fill_preserve(cr); // Fill blue and keep the rectangle.
            cairo_set_source_rgb(cr, 0.75, 0.25, 0.05);
            cairo_set_line_width(cr, 3);
            cairo_stroke(cr); // Outline the same rectangle.
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

void draw_insertion(cairo_t *cr, int width, int height, const void *state)
{
    const InsertionState *s = state;
    BubbleState adapter = {
        .values = s->values,
        .count = s->count,
        .compared = s->compared
    };
    draw_bubble(cr, width, height, &adapter);
}

// Selection Sort
// Grey bars are already sorted, the green bar is the smallest value found so far
// in this pass, and the outlined bar is the value being compared with it.
void draw_selection(cairo_t *cr, int width, int height, const void *state)
{
    const SelectionState *s = state;
    if (s->count == 0 || width <= 24 || height <= 40) return;

    double min = 0, max = 0;
    for (int i = 0; i < s->count; i++) {
        min = MIN(min, s->values[i]);
        max = MAX(max, s->values[i]);
    }

    double chart_height = height - 40.0;
    double scale = (max > min) ? chart_height / (max - min) : 0;
    double baseline = 12 + max * scale;
    double slot = (width - 24.0) / s->count;

    cairo_set_font_size(cr, 14);

    for (int i = 0; i < s->count; i++) {
        double center = 12 + (i + 0.5) * slot;
        double tip = baseline - s->values[i] * scale;
        double bar_width = slot * 0.7;

        if (i < s->i)
            cairo_set_source_rgb(cr, 0.60, 0.60, 0.60);  // Sorted
        else if (i == s->min)
            cairo_set_source_rgb(cr, 0.20, 0.65, 0.30);  // Current minimum
        else
            cairo_set_source_rgb(cr, 0.20, 0.55, 0.85);  // Unsorted

        cairo_rectangle(cr, center - bar_width / 2, MIN(tip, baseline),
                        bar_width, MAX(tip, baseline) - MIN(tip, baseline));
        if (i == s->compared) {
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, 0.75, 0.25, 0.05);
            cairo_set_line_width(cr, 3);
            cairo_stroke(cr);
        } else {
            cairo_fill(cr);
        }

        char text[16];
        g_snprintf(text, sizeof text, "%d", s->values[i]);

        cairo_text_extents_t ext;
        cairo_text_extents(cr, text, &ext);
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_move_to(cr, center - ext.width / 2 - ext.x_bearing, height - 8);
        cairo_show_text(cr, text);
    }
}

static void setup_bubble(ExplorerUI *ui) {
    List *input = ui->input_data;
    if (!ui->input_data || input->size == 0)
        return;

    g_clear_handle_id(&ui->timer_id, g_source_remove);

    int count = input->size;
    int *values = g_new(int, count);
    for (int i = 0; i < count; i++)
        values[i] = *(int *)get(input, i);

    guint selected = gtk_drop_down_get_selected(GTK_DROP_DOWN(ui->algorithm));
    if (selected == 0) {
        ui->bubble = (BubbleState){
            .values = values,
            .count = count,
            .end = count - 1,
            .compared = -1
        };
        ui->run = (Step){
            .state = &ui->bubble,
            .advance = n_bubble_step,
            .done = count < 2
        };
        ui->draw = draw_bubble;
        ui->draw_data = &ui->bubble;
    } else if (selected == 1) {
        ui->insertion = (InsertionState){
            .values = values,
            .count = count,
            .compared = -1,
            .i = count > 1 ? 1 : 0,
            .j = count > 1 ? 0 : -1,
            .saved = count > 1 ? values[1] : 0
        };
        ui->run = (Step){
            .state = &ui->insertion,
            .advance = n_insertion_step,
            .done = count < 2
        };
        ui->draw = draw_insertion;
        ui->draw_data = &ui->insertion;
    } else if (selected == 4) {
        ui->selection = (SelectionState){
            .values = values,
            .count = count,
            .compared = -1,
            .i = 0,
            .j = 1,
            .min = 0
        };
        ui->run = (Step){
            .state = &ui->selection,
            .advance = n_selection_step,
            .done = count < 2
        };
        ui->draw = draw_selection;
        ui->draw_data = &ui->selection;
    } else {
        g_free(values);
        gtk_label_set_text(GTK_LABEL(ui->status),
                           "This algorithm is not implemented yet.");
        return;
    }

    g_object_set_data_full(G_OBJECT(ui->canvas), "sort-values", values, g_free);

    update_metrics(ui);

    gtk_widget_set_sensitive(ui->start, !ui->run.done);
    gtk_widget_set_sensitive(ui->step, !ui->run.done);
    gtk_widget_set_sensitive(ui->stop, TRUE);
    gtk_widget_set_visible(ui->placeholder, FALSE);
    gtk_widget_queue_draw(ui->canvas);
}

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
                        int width, int height, gpointer data) {
    ExplorerUI *ui = data;

    if (ui->draw && ui->draw_data)
        ui->draw(cr, width, height, ui->draw_data);
}


static void set_run(ExplorerUI *ui, Step run, DrawAlgorithm draw) {
    ui->run = run;
    ui->draw = draw;

    gtk_widget_set_visible(ui->placeholder, FALSE);
    gtk_widget_set_sensitive(ui->step, !run.done);
    gtk_widget_queue_draw(ui->canvas);
}


// event listeners
static void on_step_clicked(GtkButton *button, gpointer data) {
    ExplorerUI *ui = data;

    if (!ui->run.advance || ui->run.done)
        return;

    ui->run.advance(&ui->run);
    update_metrics(ui);

    gtk_widget_queue_draw(ui->canvas);
    gtk_widget_set_sensitive(GTK_WIDGET(button), !ui->run.done);
}

static void on_apply_clicked(GtkButton *button, gpointer data)
{
    ExplorerUI *ui = data;

    const char *text = gtk_editable_get_text(GTK_EDITABLE(ui->input));

    while (g_ascii_isspace(*text))
        text++;

    if (*text == '\0')
        text = gtk_entry_get_placeholder_text(GTK_ENTRY(ui->input));

    char **parts = g_strsplit(text, ",", -1);
    int count = g_strv_length(parts);

    // Validate before replacing the saved input.
    for (int i = 0; i < count; i++) {
        gint64 value;

        if (!g_ascii_string_to_signed(g_strstrip(parts[i]), 10,
                                      G_MININT, G_MAXINT, &value, NULL)) {
            gtk_label_set_text(GTK_LABEL(ui->status),
                               "Enter integers separated by commas.");
            g_strfreev(parts);
            return;
                                      }
    }

    guint structure =
    gtk_drop_down_get_selected(GTK_DROP_DOWN(ui->structure));

    guint algorithm =
        gtk_drop_down_get_selected(GTK_DROP_DOWN(ui->algorithm));

    if (structure == 2) { // Stack

        if (!ui->input_data)
            ui->input_data = create_stack(count);
        else
            clear( ui->input_data);

        for (int i = 0; i < count; i++) {
            int value = (int)g_ascii_strtoll(parts[i], NULL, 10);
            push(&value, ((Stack*)ui->input_data)->base, sizeof value);
        }

        g_strfreev(parts);

        gtk_label_set_text(GTK_LABEL(ui->status), "Input saved.");
        Stack *stack = ui->input_data;
        setup_stack(ui);
    } else if (structure == 1) { // Queue
        if (!ui->input_data)
            ui->input_data = create_queue(count);
        else
            clear(ui->input_data);

        for (int i = 0; i < count; i++) {
            int value = (int)g_ascii_strtoll(parts[i], NULL, 10);
            enqueue( ui->input_data, &value);
        }

        g_strfreev(parts);

        gtk_label_set_text(GTK_LABEL(ui->status), "Input saved.");

        // handle queue callback
    }
    else if (structure == 0 || algorithm == 0) {
        if (!ui->input_data)
            ui->input_data = create_list(count);
        else
            clear(ui->input_data);

        for (int i = 0; i < count; i++) {
            int value = (int)g_ascii_strtoll(parts[i], NULL, 10);
            push(&value, ui->input_data, sizeof value);
        }

        g_strfreev(parts);

        gtk_label_set_text(GTK_LABEL(ui->status), "Input saved.");

        setup_bubble(ui);          // Array + Bubble Sort
    }
}

static gboolean autoplay_tick(gpointer data) {
    ExplorerUI *ui = data;

    // Same comparison, metrics update, and redraw as a manual click.
    on_step_clicked(GTK_BUTTON(ui->step), ui);

    if (ui->run.done) {
        ui->timer_id = 0;
        return G_SOURCE_REMOVE;
    }
    return G_SOURCE_CONTINUE;
}

static void on_start_clicked(GtkButton *button, gpointer data) {
    ExplorerUI *ui = data;
    if (ui->timer_id || !ui->run.advance || ui->run.done) return;

    guint delay = (guint) gtk_range_get_value(GTK_RANGE(ui->speed));
    ui->timer_id = g_timeout_add(delay, autoplay_tick, ui);
}

static void on_stop_clicked(GtkButton *button, gpointer data) {
    ExplorerUI *ui = data;
    g_clear_handle_id(&ui->timer_id, g_source_remove);
}

// helper functions

static void free_explorer(gpointer data) {
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
    (void) data;
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
    g_signal_connect(ui->stop, "clicked", G_CALLBACK(on_stop_clicked), ui);

    gtk_widget_set_sensitive(ui->step, FALSE);

    gtk_window_present(GTK_WINDOW(window));
}
