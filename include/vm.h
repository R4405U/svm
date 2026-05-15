#ifndef VM_IMPL_H
#define VM_IMPL_H

#include <stdlib.h>

typedef enum{
    OP_PUSH,
    OP_POP,
    OP_ADD,
    OP_IFEQ,
    OP_JMP,
    OP_DUP,
    OP_PRINT,
}opcode_t;

typedef struct {
    opcode_t op;
    char* arg;

}instruction_t;


void run_vm(const instruction_t* instruction, size_t program_size);

#endif
