#pragma once

#include <gtk/gtk.h>

typedef void* Any;

typedef struct {
    char *name;
    void (*action)(GtkWidget *workspace);
} Algorithm;

typedef struct {
    char *name;
    struct {
        char *name; void (*action)(void*);
    } *operations;
} Datastructure;

// array algorithms
void binary_search(GtkWidget *workspace );
void insertion_sort(GtkWidget *workspace );
void quick_sort(GtkWidget *workspace );
void bubble_sort(GtkWidget *workspace );
void merge_sort(GtkWidget *workspace );
void selection_sort(GtkWidget *workspace );


// graphs algorithms
void shortest_path(GtkWidget *workspace );
void dijkstra(GtkWidget *workspace );

// trees algorithms
void mst_prim(GtkWidget *workspace );

//
void fibonacci(GtkWidget *workspace );
