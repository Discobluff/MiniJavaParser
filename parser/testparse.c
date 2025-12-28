#include "parse.h"

Lexer *lexer_init_for_test(char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    // Get file size
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate buffer and read file
    char* source = (char*)malloc(file_size + 1);
    if (!source) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(f);
        return 1;
    }
    
    size_t bytes_read = fread(source, 1, file_size, f);
    source[bytes_read] = '\0';

    fclose(f);
    return lexer_init(source, bytes_read);
}

void test_match(Lexer* lex) { 
    Token t = match(lex, (TokenType[]) {TOKEN_CLASS, TOKENTYPE_SIZE});
    if (iserr(t)) {
        printf("error matching CLASS\n");
    } else {
        printf("matched CLASS\n");
    }
}

void test_example1() { //PASSED
    Lexer* lex = lexer_init("int x;", 6);
    /*
    int x;
    */
   Token t1 = match(lex, (TokenType[]) {TOKEN_INT, TOKENTYPE_SIZE});
    if (iserr(t1)) {
        printf("error matching INT\n");
    } else {
        printf("matched INT\n");
    }
    Token t2 = match(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
    if (iserr(t2)) {
        printf("error matching ID\n");
    } else {
        printf("matched ID\n");
    }
    Token t3 = match(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE});
    if (iserr(t3)) {
        printf("error matching PTSVG\n");
    } else {
        printf("matched PTSVG\n");  
    }
    free(lex);
}


void test_parse_file() { //PASSED
    Lexer* lex = lexer_init_for_test("tests/todo/success/basic1.java");
    File* unit = parse_file(lex);
    //freeFile(unit); problème dans freeFile OU free(lex)
    free(lex);
}

void test_parse_classwithmethod() {
    Lexer *lex = lexer_init_for_test("parser/testparser.java");
    File* unit = parse_file(lex);
    free(lex);
}


int main(int argc, char** argv) {
    test_parse_classwithmethod();
    return 0;
}