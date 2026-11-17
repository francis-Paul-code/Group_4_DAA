#pragma once

#include <gtk/gtk.h>

typedef void* Any;

typedef struct {
    char *name;
    void (*action)(GtkWidget *workspace);
} Algorithm;


typedef struct Step Step;

struct Step {
    bool done;
    void *state;
    guint64 comparisons;
    guint64 operations;
    void (*advance)(Step *step);
};

static inline void count_step(Step *step, guint64 comparisons)
{
    step->operations++;
    step->comparisons += comparisons;
}

#define print_any(x) _Generic((x), \
int:     printf("%d", *(int *){x}), \
float:   printf("%f", *(float*){x}), \
double:  printf("%lf", (x)), \
char *:  printf("%s", *(char *)(x)), \
int *:   printf("%d", *(int *)(x)), \
float *: printf("%f", *(float *)(x)), \
default: printf("%p", (void *)(x)) \
)

// array algorithms
void binary_search(GtkWidget *workspace );
void insertion_sort(GtkWidget *workspace );
void quick_sort(GtkWidget *workspace );

// bubble sort
typedef struct {
    int *values;
    int count;
    int compared;  // Left index of the last pair compared
    int i;      // Left index of the next pair
    int end;    // Last index in this pass
} BubbleState;
void n_bubble_step(Step *run);
static void draw_bubble(cairo_t *cr, int width, int height, const void *state);

void bubble_sort(GtkWidget *workspace );
void merge_sort(GtkWidget *workspace );
void selection_sort(GtkWidget *workspace );


// graphs algorithms
void shortest_path(GtkWidget *workspace );
void dijkstra(GtkWidget *workspace );

// trees algorithms
void mst_prim(GtkWidget *workspace );



// fibonacci
typedef struct {
    unsigned target; // actual n value whose fib number is required
    unsigned id;
    uint64_t current;
    uint64_t next;
} FibState;
void n_fib_step(Step *prev_step);
