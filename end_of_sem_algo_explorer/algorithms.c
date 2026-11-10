#include <gtk/gtk.h>

#include "algorithms.h"


// ------ Data Structures ---------

// 1. LIST
typedef struct {
 Datastructure base;
 Any *members;
 int size; // current number of items
 int capacity;
} List;

int push(Any item, List *list, size_t item_size) {
 if (list->size >= list->capacity) {
  size_t new_capacity = list->capacity * 2; // exponential mem allocation
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
 if (index >= list->size) return NULL; // Out of bounds safety
 return list->members[index];
}

Any remove_at_index(List *list, int index) {
 if (!get(list,index)) return NULL;
 list->members[index] = NULL;
 for (int i = index + 1; i < list->size; i++) {
  list->members[i-1] = list->members[i];
 }
 free(list->members[list->size]);
 list->size--;
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
 List *list = malloc(sizeof(List));
 if (!list) return NULL;

 list->base.operations = malloc(3 * sizeof(*list->base.operations));
 if (!list->base.operations) {
  free(list->members);
  free(list);
  return NULL;
 }

 list->base = (Datastructure){
  .name = "LIST",
  .operations = list->base.operations // Point to the allocated memory
 };

 list->base.operations[0].name = "push";
 list->base.operations[0].action = (void (*)(void*)) push;

 list->base.operations[1].name = "get";
 list->base.operations[1].action = (void (*)(void*)) get;

 list->base.operations[2].name = "clear";
 list->base.operations[2].action = (void (*)(void*)) clear;

 if (!list->members) {
  free(list);
  return NULL;
 }

 return list;
}


// 1. STACK - extends a list
typedef struct {
 List base;
} Stack;

void s_push(Any item, Stack *stack) {
 // If full, expand memory using realloc
 if (stack->size >= stack->capacity) {
  size_t new_capacity = arr->capacity * 2;
  void *new_data = realloc(arr->data, new_capacity * arr->element_size);
  if (!new_data) return 0; // Out of memory failure

  arr->data = new_data;
  arr->capacity = new_capacity;
 }

 // Calculate exactly where the new item goes in memory
 // Target address = base_address + (current_index * element_size)
 char *target = (char *) arr->data + (arr->size * arr->element_size);
 memcpy(target, item, arr->element_size);

 arr->size++;
}

Any s_peek(Stack *stack) {
}

Any s_pop(Stack *stack) {
}
