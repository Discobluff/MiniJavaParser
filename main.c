#include <lexer/lexer.h>
#include <ast/displayAst.h>
#include <error/print_error.h>
#include <parser/parse.h>
#include <typeChecker/type_checker.h>
#include <ast/ast.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int debug_lexer = 0,
    debug_parser = 0,
    show_ast = 0;

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source-file> [--debug-lexer] [--debug-parser]\n", argv[0]);
        return 1;
    }

    const char* filename = NULL;

    // Parse arguments in any order
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug-lexer") == 0) {
            debug_lexer = 1;
        } else if (strcmp(argv[i], "--debug-parser") == 0) {
            debug_parser = 1;
        } else if (strcmp(argv[i], "--show-ast") == 0) {
            show_ast = 1;
        } else if (argv[i][0] != '-') {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, "Error: No source file specified\n");
        return 1;
    }
    
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
    
    if (debug_lexer) printf("lexer initialized with %d declared type(s)\n", lex->decl_type_list_size);

    for (int i = 0; i < lex->decl_type_list_size * debug_lexer; i++) {
        char buf[32];
        token_identifier(lex, (Token){.type = TOKEN_ID_TYPE, .position = lex->decl_type_list[i]}, buf, 32);
        printf("Declared type %d: %s\n", i, buf);
    }

    if(debug_lexer) {
        printf("Lexing file: \"%s\"\n\n", filename);

        while (lex->tok.type != TOKEN_EOF) {
            token_pprint(lex->tok, lex);
            lexer_next(lex);
        }
        lex = lexer_init(source, bytes_read);
    }

    File* unit = parse_file(lex);

    if (show_ast){
        AST ast = (AST){ .type = AST_FILE, .data.file = unit };
        mainloop(ast);
    }

    //type_check_file(unit); //##### TYPECHECKER DISABLED FOR NOW #####

    freeFile(unit);
    free(source);
    lexerfree(lex);
    return 0;
}
