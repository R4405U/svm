#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "common.h"


// Helper functions

static int is_alphanumeric(const char c){
    return isalnum((unsigned char)c) || c == '_';
}

static char peek_char(const lexer_state_t* state){
    return state->source[state->current];
}

static char peek_next_char(const lexer_state_t* state){
    if(state->source[state->current] == '\0') return '\0';

    return state->source[state->current+1];
}

static token_t make_token(token_type_t type, const lexer_state_t* state){
    token_t token;
    token.type = type;
    token.line = state->line;

    int length = state->current - state->start;

    token.lexeme = (char*)malloc(length+1);
    check_null(token.lexeme, "Lexer Error: Memory allocation failed for lexeme string");

    strncpy(token.lexeme, &state->source[state->start],length);
    token.lexeme[length] = '\0';

    return token;

}

static token_type_t match_keyword(const char* lexeme){
    if (strcmp(lexeme, "PUSH") == 0)   return TOKEN_PUSH;
    if (strcmp(lexeme, "POP") == 0)    return TOKEN_POP;
    if (strcmp(lexeme, "DUP") == 0)    return TOKEN_DUP;
    if (strcmp(lexeme, "SWAP") == 0)   return TOKEN_SWAP;
    if (strcmp(lexeme, "OVER") == 0)   return TOKEN_OVER;
    if (strcmp(lexeme, "NIP") == 0)    return TOKEN_NIP;
    if (strcmp(lexeme, "TUCK") == 0)   return TOKEN_TUCK;
    if (strcmp(lexeme, "PICK") == 0)   return TOKEN_PICK;
    if (strcmp(lexeme, "ADD") == 0)    return TOKEN_ADD;
    if (strcmp(lexeme, "SUB") == 0)    return TOKEN_SUB;
    if (strcmp(lexeme, "MUL") == 0)    return TOKEN_MUL;
    if (strcmp(lexeme, "DIV") == 0)    return TOKEN_DIV;
    if (strcmp(lexeme, "IFEQ") == 0)   return TOKEN_IFEQ;
    if (strcmp(lexeme, "JMP") == 0)    return TOKEN_JMP;
    if (strcmp(lexeme, "HALT") == 0)   return TOKEN_HALT;
    if (strcmp(lexeme, "PRINT") == 0)  return TOKEN_PRINT;

    return TOKEN_IDENTIFIER; // Fallback: Must be a jump target reference label name
}


void lexer_init(lexer_state_t* state, const char* source_code){
   check_null((void*)source_code, "Lexer Error: Passed NULL source string to lexer_init");
    state->source = source_code;
    state->start = 0;
    state->current = 0;
    state->line = 1;

}

token_t next_token(lexer_state_t* state){

    while(state->source[state->current] != '\0'){
        state->start = state->current;

        char c = state->source[state->current];

        // skip whitespaces
        if(c == ' ' || c == '\r' || c == '\t'){
            state->current++;
            continue;
        }
        // skip newline and increment line
        if(c == '\n'){
            state->line++;
            state->current++;
            continue;
        }

        // skip comments
        if(c == ';'){
            while(peek_char(state) != '\n' && peek_char(state) != '\0'){
                state->current++;
            }
            continue;
        }

        // Scan Digits (Numeric Litrels)
        if(isdigit((unsigned char)c) || (c == '-' && isdigit((unsigned char)peek_next_char(state)))){
            state->current++;
        while(isdigit((unsigned char)peek_char(state))){
            state->current++;
        }

        return make_token(TOKEN_INTEGER, state);
        }

        // scan words

        if(isalpha((unsigned char)c) || c == '_'){
            state->current++;
            while(is_alphanumeric(peek_char(state))){
                state->current++;
            }

            if(peek_char(state) == ':'){
                state->current++;
                return make_token(TOKEN_LABEL,state);
            }

            token_t token = make_token(TOKEN_IDENTIFIER, state);
            token.type = match_keyword(token.lexeme);
            return token;
        }
        // unrecogonized character
        state->current++;
        return make_token(TOKEN_ERROR,state);
    }
    // end of file
    state->start = state->current;
    return make_token(TOKEN_EOF, state);
}

void free_token(token_t token){
    if(token.lexeme != NULL){
        free(token.lexeme);
    }
}
