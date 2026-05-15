#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "vm.h"



const char* usage(void){
	const char* usage = "Usage: sm [filename]\n";
	return usage;
}

// Helper to free the memory allocated for instruction arguments
void free_program(instruction_t* program, size_t size){
    for (size_t i = 0; i < size; i++) {
        if (program[i].arg != NULL) {
            free(program[i].arg);
        }
    }
    free(program);
}

// Helper function to read file
const char* read_file(const char* filename){
	FILE* fptr = fopen(filename, "r");
	fseek(fptr, 0, SEEK_END);
	long filesize = ftell(fptr);
	rewind(fptr);

	char* source_code = malloc(filesize + 1);
	fread(source_code, filesize, 1, fptr);
	fclose(fptr);

	source_code[filesize] = 0;
	return source_code;
}

const char* parse_arguments(int argc, char const *argv[]){
	if(argc <= 1){
		fprintf(stderr, "sm: no input file provided\n%s",
			usage());
		exit(EXIT_FAILURE);
	}
	if(strncmp(argv[1], "--help", 6) == 0){
		printf("%s\n", usage());
		exit(0);
	}
	return argv[1];
}


int main(int argc, char const *argv[]){


	const char* filename = parse_arguments(argc, argv);
	const char* source_code = read_file(filename);

	if(source_code == NULL){
		fprintf(stderr,"sm: file does not exist or cannot be opened.\n");
		return 1;
	}

    size_t program_size = 0;
    instruction_t* compiled_bytecode = parse_program(source_code, &program_size);

	free((void*)source_code);

    run_vm(compiled_bytecode, program_size);

    free_program(compiled_bytecode, program_size);

	return 0;
}
