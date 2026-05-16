#ifndef LEXER_IMPL_H
#define LEXER_IMPL_H

typedef enum {
  TOKEN_PUSH,
  TOKEN_POP,
  TOKEN_DUP,
  TOKEN_SWAP,
  TOKEN_OVER,


  TOKEN_ADD,
  TOKEN_SUB,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_NIP,
  TOKEN_TUCK,
  TOKEN_PICK,


  TOKEN_IFEQ,
  TOKEN_JMP,
  TOKEN_HALT,
  TOKEN_PRINT,

  TOKEN_INTEGER,

  TOKEN_IMPORT,
  TOKEN_IDENTIFIER,
  TOKEN_LABEL,

  TOKEN_EOF,
  TOKEN_ERROR,

} token_type_t;

typedef struct {
  token_type_t type;
  char* lexeme;
  int line;
} token_t;

typedef struct {
  const char* source; // points to raw file buffer loaded from disk;
  int start;   // Index tracking the beginning of the token currently being
               // scanned
  int current; // Index tracking the current character under inspection
  int line;    // Current line number tracking (for compiler error messages)
} lexer_state_t;

void lexer_init(lexer_state_t* state, const char* source_code);
token_t next_token(lexer_state_t* state);
void free_token(token_t token);

#endif
