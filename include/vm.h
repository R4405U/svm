#ifndef VM_IMPL_H
#define VM_IMPL_H

#include <stdlib.h>

typedef enum{
    OP_PUSH,
    OP_POP,
    OP_DUP,
    OP_SWAP,
    OP_OVER,
    OP_NIP,
    OP_TUCK,
    OP_PICK,

    
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,


    OP_IFEQ,
    OP_JMP,
    OP_HALT,
    OP_PRINT,
}opcode_t;

typedef struct {
    opcode_t op;
    char* arg;

}instruction_t;


void run_vm(const instruction_t* instruction, size_t program_size);

#endif
