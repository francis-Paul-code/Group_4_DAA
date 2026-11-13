#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "datastructures.h"

// 1. LIST

int push(Any item, List *list, size_t item_size) {
 if (!list || !item || !item_size) return 0;

 if (list->size >= list->capacity) {
  if (list->capacity > INT_MAX / 2) return 0;
  size_t new_capacity = list->capacity ? list->capacity * 2 : 1; // exponential mem allocation
  void *new_data = realloc(list->members, new_capacity * sizeof(Any));
  if (!new_data) return 0;

  list->members = new_data;
  list->capacity = new_capacity;
 }

 // create a memory slice for the new item with the same size as the item
 void *item_copy = malloc(item_size);
 if (!item_copy) return 0;

 // Copy the source content into the unique slice
 memcpy(item_copy, item, item_size);

 // 4. Save the item's pointer into our pointer array
 list->members[list->size] = item_copy;
 list->size++;

 return 1;
}

Any get(List *list, int index) {
 if (!list || index < 0 || index >= list->size) return NULL; // Out of bounds safety
 return list->members[index];
}

void printList(List *list) {
 if (!list) return;

 printf("List:  [ ");
 for (int i = 0; i < list->size; i++) {
  printf("%p", get(list, i));
  printf(", ");
 }
 printf("] \n");
}

Any remove_at_index(List *list, int index) {
 if (!list || index < 0 || index >= list->size) return NULL;
 Any removed = list->members[index];
 for (int i = index + 1; i < list->size; i++) {
  list->members[i - 1] = list->members[i];
 }
 list->members[--list->size] = NULL;
 return removed;
}

void clear(List *list) {
 if (!list) return;

 for (int i = 0; i < list->size; i++) {
  free(list->members[i]); // Free each object
 }
 free(list->members); // Free the pointer array itself
 free(list); // Free the array struct
}

List *create_list(size_t initial_capacity) {
 if (initial_capacity > INT_MAX) return NULL;
 if (!initial_capacity) initial_capacity = 1;
 List *list = malloc(sizeof(List));
 if (!list) return NULL;

 list->base = (Datastructure){
  .name = "LIST",
 };

 list->capacity = initial_capacity;
 list->size = 0;
 list->members = malloc(initial_capacity * sizeof(Any));

 if (!list->members) {
  free(list);
  return NULL;
 }

 return list;
}


// 1. STACK - extends a list

Stack *create_stack(size_t initial_capacity) {
 Stack *stack = malloc(sizeof(Stack));
 if (!stack) return NULL;

 List *list = create_list(initial_capacity);
 if (!list) {
  free(stack);
  return NULL;
 }

 list->base = (Datastructure){
  .name = "Stack"
 };

 stack->base = list;

 return stack;
}

Any s_peek(Stack *stack) {
 if (!stack) return NULL;
 return get(stack->base, stack->base->size - 1);
}

Any s_pop(Stack *stack) {
 if (!stack) return NULL;
 return remove_at_index(stack->base, stack->base->size - 1);
}

void s_clear(Stack *stack) {
 if (!stack) return;
 clear(stack->base);
 free(stack);
}

// 3. Queue - extends a list

Queue *create_queue(size_t initial_capacity) {
 Queue *q = malloc(sizeof(Queue));
 if (!q) return NULL;

 List *list = create_list(initial_capacity);
 if (!list) {
  free(q);
  return NULL;
 }

 list->base = (Datastructure){
  .name = "Queue"
 };

 q->base = list;

 return q;
};

Any dequeue(Queue *q) {
 if (!q) return NULL;
 return remove_at_index(q->base, 0);
};

void enqueue(Queue *q, Any item) {
 if (!q) return;
 push(item, q->base, sizeof(item));
};

Any q_peek(Queue *q) {
 if (!q) return NULL;
 return get(q->base, 0);
}

// Graph

Vertex *create_vertex(Any data) {
 Vertex *v = malloc(sizeof(Vertex));
 if (!v) return NULL;
 v->data = data;
 v->edges = create_list(2);
 if (!v->edges) {
  free(v);
  return NULL;
 }
 return v;
}

Edge *create_edge(Vertex *a, Vertex *b) {
 if (!a || !b) return NULL;
 Edge *e = malloc(sizeof(Edge));
 if (!e) return NULL;
 *e = (Edge){.a = a, .b = b};
 if (!add_edge(a, e)) {
  free(e);
  return NULL;
 }
 if (a != b && !add_edge(b, e)) {
  free(remove_at_index(a->edges, a->edges->size - 1));
  free(e);
  return NULL;
 }
 return e;
}

Vertex *add_edge(Vertex *v, Edge *e) {
 if (!v || !e || (e->a != v && e->b != v)) return NULL;
 if (!push(e, v->edges, sizeof(*e))) return NULL;
 return v;
}

// Store vertex pointers in a temporary list so cycles are visited only once.
static int collect_vertices(Vertex *v, List *visited, bool print) {
 if (!v) return 1;
 // check if vertex has been visited to avoid infinte loop
 for (int i = 0; i < visited->size; i++) {
  if (*(Vertex **)get(visited, i) == v) return 1;
 }

 if (!push(&v, visited, sizeof(v))) return 0;

 if (print) print_any(v->data);

 for (int i = 0; i < v->edges->size; i++) {
  Edge *e = get(v->edges, i);
  Vertex *next = e->a == v ? e->b : e->a;
  // recursively visit the next vertex unti end
  if (!collect_vertices(next, visited, print)) return 0;
 }
 return 1;
}

void traverse(Vertex *v) {
 List *visited = create_list(2);
 if (!visited) return;
 collect_vertices(v, visited, true);
 printf("\n");
 clear(visited);
}

void print_graph(Vertex *v) {
 List *visited = create_list(2);
 if (!visited) return;
 if (collect_vertices(v, visited,false)) {
  for (int i = 0; i < visited->size; i++) {
   Vertex *vertex = *(Vertex **)get(visited, i);
   printf("Vertex %p (data %p): [ ", (void *)vertex, vertex->data);
   for (int j = 0; j < vertex->edges->size; j++) {
    Edge *e = get(vertex->edges, j);
    Vertex *next = e->a == vertex ? e->b : e->a;
    printf("%p ", (void *)next);
   }
   printf("]\n");
  }
 }
 clear(visited);
}

void clear_graph(Vertex *v) {
 List *visited = create_list(2);
 if (!visited) return;
 if (collect_vertices(v, visited,false)) {
  for (int i = 0; i < visited->size; i++) {
   Vertex *vertex = *(Vertex **)get(visited, i);
   clear(vertex->edges);
   free(vertex);
  }
 }
 clear(visited);
}

// Tree - extends a list of child pointers

Tree *create_tree(Any data) {
 Tree *tree = malloc(sizeof(Tree));
 if (!tree) return NULL;
 tree->data = data;
 tree->parent = NULL;
 tree->children = create_list(2);
 if (!tree->children) {
  free(tree);
  return NULL;
 }
 return tree;
}

Tree *add_child(Tree *parent, Tree *child) {
 if (!parent || !child || child->parent) return NULL;
 // Attaching an ancestor would introduce a cycle.
 for (Tree *node = parent; node; node = node->parent) {
  if (node == child) return NULL;
 }
 if (!push(&child, parent->children, sizeof(child))) return NULL;
 child->parent = parent;
 return parent;
}

Tree *remove_child(Tree *parent, int index) {
 if (!parent) return NULL;
 Tree **entry = remove_at_index(parent->children, index);
 if (!entry) return NULL;
 Tree *child = *entry;
 child->parent = NULL;
 free(entry);
 return child;
}

void traverse_tree(Tree *tree) {
 if (!tree) return;
 printf("%p\n", tree->data);
 for (int i = 0; i < tree->children->size; i++) {
  traverse_tree(*(Tree **)get(tree->children, i));
 }
}

static void print_tree_level(Tree *tree, int depth) {
 if (!tree) return;
 for (int i = 0; i < depth; i++) printf("  ");
 printf("%p\n", tree->data);
 for (int i = 0; i < tree->children->size; i++) {
  print_tree_level(*(Tree **)get(tree->children, i), depth + 1);
 }
}

void print_tree(Tree *tree) {
 print_tree_level(tree, 0);
}

void clear_tree(Tree *tree) {
 if (!tree) return;
 if (tree->parent) {
  Tree *parent = tree->parent;
  for (int i = 0; i < parent->children->size; i++) {
   if (*(Tree **)get(parent->children, i) == tree) {
    remove_child(parent, i);
    break;
   }
  }
 }
 while (tree->children->size) {
  clear_tree(remove_child(tree, tree->children->size - 1));
 }
 clear(tree->children);
 free(tree);
}
