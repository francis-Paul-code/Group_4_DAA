#pragma once

#include <stddef.h>

typedef void *Any;

typedef struct {
    char *name;
} Datastructure;

typedef struct {
    Datastructure base;
    Any *members;
    int size; // current number of items
    int capacity;
} List;

typedef struct {
    List *base;
} Stack;

typedef struct {
    List *base;
} Queue;

#define print_any(x) _Generic((x), \
int:     printf("%d", *(int *){x}), \
float:   printf("%f", *(float*){x}), \
double:  printf("%lf", (x)), \
char *:  printf("%s", *(char *)(x)), \
int *:   printf("%d", *(int *)(x)), \
float *: printf("%f", *(float *)(x)), \
default: printf("%p", (void *)(x)) \
)

// List
List *create_list(size_t inital_capacity);
int push(Any item, List *list, size_t item_size);
Any get(List *list, int index);
Any remove_at_index(List *list, int index);
void clear(List *list);
void printList(List *list);

// Stack
Stack *create_stack(size_t inital_capacity);
Any s_peek(Stack *stack);
Any s_pop(Stack *stack);
void s_clear(Stack *stack);

// Queue
Queue *create_queue(size_t inital_capacity);
Any dequeue(Queue *q);
void enqueue(Queue *q, Any item);
Any q_peek(Queue *q);

// Graph

typedef struct {
    Any data;
    List *edges; // list of edges connected to a vertex
} Vertex;

typedef struct {
    Vertex *a;
    Vertex *b;
} Edge;

Vertex* create_vertex(Any data);
Edge* create_edge(Vertex* a, Vertex *b);
Vertex* add_edge(Vertex* v, Edge* e);
void traverse(Vertex *v);
void print_graph(Vertex *v);
void clear_graph(Vertex *v);

// Tree - each node owns its children, but borrows its data.
typedef struct Tree Tree;
struct Tree {
    Any data;
    List *children; // list of copied Tree pointers
    Tree *parent;
};

Tree *create_tree(Any data);
Tree *add_child(Tree *parent, Tree *child);
Tree *remove_child(Tree *parent, int index); // transfers ownership to caller
void traverse_tree(Tree *tree);
void print_tree(Tree *tree);
void clear_tree(Tree *tree);
