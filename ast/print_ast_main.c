#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer/lexer.h>
#include <parser/parse.h>
#include "print_ast.h"

static Lexer* lexer_init_from_file(const char *filename){
    FILE* f = fopen(filename, "r");
    if (!f){
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* source = malloc(file_size + 1);
    if (!source){ fclose(f); return NULL; }
    size_t bytes_read = fread(source, 1, file_size, f);
    source[bytes_read] = '\0';
    fclose(f);
    return lexer_init(source, (int)bytes_read);
}

int main(int argc, char **argv){
    if (argc < 2){
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    Lexer *lex = lexer_init_from_file(filename);
    if (!lex) return 2;
    File *unit = parse_file(lex);
    if (!unit){
        fprintf(stderr, "Parsing failed or returned NULL\n");
        lexerfree(lex);
        return 3;
    }
    print_file_ast(unit);
    freeFile(unit);
    lexerfree(lex);
    return 0;
}
