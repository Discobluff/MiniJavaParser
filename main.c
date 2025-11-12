#include "lexer/lexer.h"
#include "error/print_error.h"
#include "parser/parse.h"
#include "ast/ast.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage: %s <source-file>\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    
    // Open the file
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return 1;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer and read file
    char* source = (char*)malloc(file_size + 1);
    if (!source) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }
    
    size_t bytes_read = fread(source, 1, file_size, file);
    source[bytes_read] = '\0';

    fclose(file);
    
    // Initialize lexer with file contents
    Lexer* lex = lexer_init(source, bytes_read);
    printf("Lexing file: \"%s\"\n\n", filename);

    while (lex->tok.type != TOKEN_EOF) {
        token_pprint(lex->tok, lex);
        lexer_next(lex);
    }
    

    // lex = lexer_init(source, bytes_read);

    // AST* ast = parse_main(lex);

    // print_ast(ast, 0);
    
    // free_ast(ast);

    free(source);
    free(lex);
    return 0;
}
