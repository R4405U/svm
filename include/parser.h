#ifndef PARSER_IMPL_H
#define PARSER_IMPL_H

#include "common.h"
#include "lexer.h"
#include "vm.h"

// Pass 1 parser
// maps labels to the correct address (PC) in symbol table
int parser_pass_one(symbol_table_t* table, const char* source_code);

// Maps a keyword token type to its matching native VM operation enum
opcode_t token_to_opcode(token_type_t type);

// Combines Pass 1 and Pass 2 to output the VM program
instruction_t* parse_program(const char* source_code, size_t* out_program_size);

#endif