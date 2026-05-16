#include "parser.h"
#include "common.h"
#include "lexer.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parser_pass_one(symbol_table_t *table, const char *source_code) {

  check_null((void *)table, "Parser Error: Passed NULL table to parser_pass1");
  check_null((void *)source_code,
             "Parser Error: Passed NULL lexer state to parser_pass1");

  lexer_state_t state;
  lexer_init(&state, source_code);

  int instructin_count = 0;
  token_t token = next_token(&state);

  while (token.type != TOKEN_EOF) {

    if (token.type == TOKEN_ERROR) {
      fprintf(stderr, "Syntax Error: Unrecognized token '%s' on line %d\n",
              token.lexeme, token.line);
      free_token(token);
      exit(EXIT_FAILURE);
    }

    if (token.type == TOKEN_LABEL) {
      int len = strlen(token.lexeme);
      if (len > 0 && token.lexeme[len - 1] == ':') {
        token.lexeme[len - 1] = '\0';
      }

      symb_table_insert(table, token.lexeme, instructin_count);
    } else if (token.type != TOKEN_INTEGER && token.type != TOKEN_IDENTIFIER) {

      instructin_count++;
    }

    free_token(token);
    token = next_token(&state);
  }
  free_token(token);
  return instructin_count;
}


opcode_t token_to_opcode(token_type_t type) {
    switch (type) {
        case TOKEN_PUSH:  return OP_PUSH;
        case TOKEN_POP:   return OP_POP;
        case TOKEN_DUP:   return OP_DUP;
        case TOKEN_SWAP:  return OP_SWAP;
        case TOKEN_OVER:  return OP_OVER;
        case TOKEN_NIP:   return  OP_NIP;
        case TOKEN_TUCK:  return OP_TUCK;
        case TOKEN_PICK:  return OP_PICK;
        case TOKEN_ADD:   return OP_ADD;
        case TOKEN_SUB:   return OP_SUB;
        case TOKEN_MUL:   return OP_MUL;
        case TOKEN_DIV:   return OP_DIV;
        case TOKEN_IFEQ:  return OP_IFEQ;
        case TOKEN_JMP:   return OP_JMP;
        case TOKEN_HALT:  return OP_HALT;
        case TOKEN_PRINT: return OP_PRINT;
        default:
            fprintf(stderr, "Compiler Error: Token type %d cannot be mapped to an opcode.\n", type);
            exit(EXIT_FAILURE);
    }
}

instruction_t* parse_program(const char* source_code, size_t* out_program_size){
    check_null((void*)source_code, "Compiler Error: Passed NULL source code to parse_program");
    check_null((void*)out_program_size, "Compiler Error: Passed NULL output size pointer");

    // Pass 1
    symbol_table_t symb_table;
    symb_table_init(&symb_table);

    int total_instructions = parser_pass_one(&symb_table, source_code);

    // Allocate memory for the bytecode block based on Pass 1 sizing
    instruction_t* bytecode = (instruction_t*)malloc(sizeof(instruction_t) * total_instructions);
    check_null(bytecode, "Compiler Error: Failed to allocate instruction block memory");
    *out_program_size = (size_t)total_instructions;

    // Reset lexer state back to index 0 for Pass 2 processing
    lexer_state_t state;
    lexer_init(&state, source_code);

    int pc = 0;
    token_t token = next_token(&state);

    while(token.type != TOKEN_EOF){

        // Skip label declarations entirely in Pass 2 (already resolved)
        if(token.type == TOKEN_LABEL){
            free_token(token);
            token = next_token(&state);
            continue;
        }
        
        // check if token is valid instruction keyword
        if(token.type != TOKEN_INTEGER && token.type != TOKEN_IDENTIFIER){
            bytecode[pc].op = token_to_opcode(token.type);
            bytecode[pc].arg = NULL;

            // Lookahead: Determine if this specific opcode requires an argument
            if(token.type == TOKEN_PUSH || 
                token.type == TOKEN_PICK || 
                token.type == TOKEN_JMP || 
                token.type == TOKEN_IFEQ){

                token_t token_arg = next_token(&state);
                if(token_arg.type != TOKEN_INTEGER && token_arg.type != TOKEN_IDENTIFIER){
                    fprintf(stderr, "Compilation Error [Line %d]: Expected argument after instruction keyword.\n", token_arg.line);
                    exit(EXIT_FAILURE);
                }
                
                if(token.type == TOKEN_IDENTIFIER){
                    int target_address = symb_table_lookup(&symb_table, token_arg.lexeme);
                    if(target_address == -1){
                        fprintf(stderr, "Compilation Error [Line %d]: Reference to undefined label '%s'\n", 
                                token_arg.line, token_arg.lexeme);
                        exit(EXIT_FAILURE);
                    }

                    char buffer[32];
                    sprintf(buffer, "%d", target_address);
                    bytecode[pc].arg = strdup(buffer);
                } else {
                    bytecode[pc].arg = strdup(token_arg.lexeme);
                }
                check_null(bytecode[pc].arg, "Compiler Error: Allocation failed cloning argument string");
                free_token(token_arg);
            }
            pc++;
        }
        free_token(token);
        token = next_token(&state);
    }
    free_token(token);          // Clean up the final TOKEN_EOF allocation
    symb_table_free(&symb_table); // Clean up the symbol table allocations cleanly
    return bytecode;
}





