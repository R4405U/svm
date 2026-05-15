#ifndef COMMON_IMPL_H
#define COMMON_IMPL_H

#define MAX_ENTRIES 256

#include <stdlib.h>


// Linked List

typedef struct node{
    char* data;
    struct node* next;
}node_t;


typedef struct{
   node_t* head;
}ll_t;

node_t* create_node(const char* value);

ll_t* create_ll(const char* data);
void insert_at_head(ll_t* list,const char* data);
char* remove_from_head(ll_t* list);
char* value_at(const ll_t* list, const unsigned int index);
size_t lengthof_ll(const ll_t* list);

// Stack

typedef struct {
    ll_t* list;
}stack_t;

stack_t* create_stack(void);
void push(stack_t* stack, const char* data);
char* pop(stack_t* stack);
const char* peek(const stack_t* stack);
void free_stack(stack_t* stack);


typedef struct{
    char* name;     // Label name (eg: "loop")
    int address;    // The exact instruction index (PC) it points to
}entry_t;


typedef struct{
    entry_t entries[MAX_ENTRIES];
    int count;      // Total number of registered entries
}symbol_table_t;

// Initialize or reset the symbol table state
void symb_table_init(symbol_table_t* table);

//  Insert a new label and its corresponding instruction index
void symb_table_insert(symbol_table_t* table, const char* name, int address);

//  Search for a label and return its address (-1 if not found)
int symb_table_lookup(const symbol_table_t* table, const char* name);

//  Free all heap-allocated label names within the table
void symb_table_free(symbol_table_t* table);





// Helper functions
void check_null(void* val, const char* msg);



#endif
