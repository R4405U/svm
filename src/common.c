#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_null(void *val, const char *msg) {
  if (val == NULL) {
    perror(msg);
    exit(1);
  }
}

// Linked List implementation

node_t *create_node(const char *value) {
  if (value == NULL)
    return NULL;

  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  check_null(new_node, "Memory allocation error");

  new_node->data = strdup(value);
  if (new_node->data == NULL) {
    fprintf(stderr, "String Duplication Failed\n");
    free(new_node);
    exit(1);
  }
  new_node->next = NULL;
  return new_node;
}

ll_t *create_ll(const char *data) {
  ll_t *heap_list = (ll_t *)malloc(sizeof(ll_t));

  check_null(heap_list, "Failed to allocate memory");

  heap_list->head = create_node(data);
  return heap_list;
}

void insert_at_head(ll_t *list, const char *data) {
  check_null((void *)list, "Passed a NULL list pointer");
  node_t *new_node = create_node(data);
  new_node->next = list->head;
  list->head = new_node;
}

char *remove_from_head(ll_t *list) {

  check_null((void *)list, "Passed a NULL list pointer");

  node_t *current_head_node = list->head;
  char *current_head_node_data = list->head->data;
  list->head = current_head_node->next;
  free(current_head_node);
  return current_head_node_data;
}

char *value_at(const ll_t *list, const unsigned int index) {

  check_null((void *)list, "Passed a NULL list pointer");

  node_t *current = list->head;
  unsigned int current_index = 0;

  while (current != NULL) {
    if (current_index == index) {
      return current->data;
    }
    current_index++;
    current = current->next;
  }

  fprintf(stderr, "Index %u out of bounds\nlist size is %u", index,
          current_index);
  exit(1);
}

size_t lengthof_ll(const ll_t *list) {
  check_null((void *)list, "Passed a NULL list pointer");

  size_t count = 0;
  node_t *current = list->head;

  while (current != NULL) {
    count++;
    current = current->next;
  }

  return count;
}

// Stack implementation

stack_t *create_stack(void) {
  stack_t *heap_stack = (stack_t *)malloc(sizeof(stack_t));

  heap_stack->list = create_ll(NULL);

  return heap_stack;
}

void push(stack_t *stack, const char *data) {
  insert_at_head(stack->list, data);
}

char *pop(stack_t *stack) { return remove_from_head(stack->list); }

const char *peek(const stack_t *stack) {
  if (stack->list->head == NULL) {
    return NULL;
  }
  return stack->list->head->data;
}

void free_stack(stack_t *stack) {
  if (stack == NULL)
    return;

  check_null((void *)stack->list, "Passed a NULL list pointer");

  node_t *current = stack->list->head;

  while (current != NULL) {
    node_t *next_node = current->next;

    if (current->data != NULL) {
      free(current->data);
    }
    free(current);
    current = next_node;
  }
  free(stack->list);
  free(stack);
}

// Symbol table

void symb_table_init(symbol_table_t *table) {
  check_null((void *)table,
             "Parser Error: Passed NULL pointer to sym_table_init");
  table->count = 0;

  memset(table->entries, 0, sizeof(entry_t) * MAX_ENTRIES);
}

int symb_table_lookup(const symbol_table_t *table, const char *name) {
  check_null((void *)table,
             "Parser Error: Passed NULL table to sym_table_lookup");
  check_null((void *)name,
             "Parser Error: Passed NULL name to sym_table_lookup");

  for (int i = 0; i < table->count; i++) {
    if (strcmp(table->entries[i].name, name) == 0) {
      return table->entries[i].address;
    }
  }
  return -1;
}

void symb_table_insert(symbol_table_t *table, const char *name, int address) {
  check_null((void *)table,
             "Parser Error: Passed NULL table to sym_table_insert");
  check_null((void *)name,
             "Parser Error: Passed NULL name to sym_table_insert");

  if (table->count >= MAX_ENTRIES) {
    fprintf(stderr,
            "Compilation Error: Symbol table limit exceeded (%d labels).\n",
            MAX_ENTRIES);
    exit(EXIT_FAILURE);
  }

  if (symb_table_lookup(table, name) != -1) {
    fprintf(stderr,
            "Compilation Error: Duplicate label definition detected: '%s'\n",
            name);
    exit(EXIT_FAILURE);
  }

  char *name_copy = strdup(name);
  check_null(name_copy,
             "Parser Error: Allocation failed cloning label identifier string");

  table->entries[table->count].name = name_copy;
  table->entries[table->count].address = address;
  table->count++;
}

void symb_table_free(symbol_table_t *table) {
  check_null((void *)table,
             "Parser Error: Passed NULL table to sym_table_free");
  for (int i = 0; i < table->count; i++) {
    if (table->entries[i].name != NULL) {
      free(table->entries[i].name);
    }
  }
  table->count = 0;
}
