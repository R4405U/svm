#include <stdio.h>
#include <string.h>
#include "vm.h"
#include "common.h"


void run_vm(const instruction_t* instructions, size_t program_size){

    if(instructions == NULL || program_size == 0) return;

    stack_t* vm_stack = create_stack();

    int pc = 0;
    int running = 1;

    while(running && (size_t)pc < program_size){

        // Debug Message;
       //printf("[DEBUG] PC: %d, OP: %d, ARG: %s\n", pc, instructions[pc].op, instructions[pc].arg ? instructions[pc].arg : "NULL");





        instruction_t current_inst = instructions[pc];

        switch(current_inst.op){

            case OP_PUSH: {
                char* data_copy = strdup(current_inst.arg);
                check_null(data_copy, "VM Error: strdup failed during OP_PUSH");
                push(vm_stack, data_copy);
                pc++;
                break;
            }

            case OP_POP: {
                char* popped = pop(vm_stack);
                if(popped == NULL){
                    fprintf(stderr, "Runtime Error: Stack underflow on OP_POP at PC %d\n", pc);
                    exit(EXIT_FAILURE);
                }
                free(popped);
                pc++;
                break;
            }

            case OP_DUP: {
                const char* top_value = peek(vm_stack);
                if(top_value == NULL){
                    fprintf(stderr, "Runtime Error: Stack underflow on OP_DUP at PC %d\n", pc);
                    exit(EXIT_FAILURE);
                }
                push(vm_stack, top_value);
                pc++;
                break;
            }

            case OP_ADD: {
                char* val2_str = pop(vm_stack);

                char* val1_str = pop(vm_stack);

                if (val1_str == NULL || val2_str == NULL) {
                    fprintf(stderr, "Runtime Error: Stack underflow on OP_ADD at PC %d\n", pc);
                    exit(EXIT_FAILURE);
                }

                int val1 = atoi(val1_str);
                int val2 = atoi(val2_str);
                int result = val1 + val2;


                char result_str[32];
                snprintf(result_str,sizeof(result_str),"%d",result);

                char* heap_result = strdup(result_str);

                push(vm_stack, heap_result);

                free(val1_str);
                free(val2_str);
                pc++;
                break;
            }

            case OP_PRINT: {
                const char* top_value = peek(vm_stack);

                if(top_value != NULL){
                    printf("%s\n", top_value);
                    fflush(stdout); // Force text out of the buffer immediately
                }
                pc++;
                break;
            }

            case OP_JMP: {
                if(current_inst.arg == NULL){
                    fprintf(stderr, "Runtime Error: OP_JMP missing target address at PC %d\n", pc);
                    exit(EXIT_FAILURE);
                }
                pc = atoi(current_inst.arg);
                break;
            }

            case OP_IFEQ: {
                if(current_inst.arg == NULL){
                    fprintf(stderr, "Runtime Error: OP_IFEQ missing target address at PC %d\n", pc);
                    exit(EXIT_FAILURE);
                }

                char* condition_str = pop(vm_stack);
                if (condition_str == NULL) {
                    fprintf(stderr, "Runtime Error: Stack underflow on OP_IFEQ at PC %d\n", pc);
                    exit(EXIT_FAILURE);
                }

                int condition_val = atoi(condition_str);

                free(condition_str);

                if(condition_val == 0){
                    pc = atoi(current_inst.arg);
                } else {
                    pc++;
                }
                break;
            }

            default:
                fprintf(stderr, "Runtime Error: Unknown Opcode %d at PC %d\n", current_inst.op, pc);
                exit(EXIT_FAILURE);

        }
    }
    free_stack(vm_stack);
}
