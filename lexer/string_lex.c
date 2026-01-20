#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


#include <lexer/lexer.h>
#include <lexer/string_lex.h>
#include <error/print_error.h>


bool good_string(Lexer* lex) {
    // We just read " in the lexer
    lex->pos++;
    
    while (lex->pos < lex->length) {
        char c = lex->source[lex->pos];
        
        if (c == '\n') {
            char msg[64];
            snprintf(msg, sizeof(msg), "Newline in string literal");
            lex->tok.position = lex->pos;
            error_print(lex, msg); 
            exit(1);
            return false;
        }
        
        if (c == '\\') {
            /* backslash: consume it and the escaped character, ignoring escaped quotes */
            if (lex->pos == lex->length - 1) {
                char msg[64];
                snprintf(msg, sizeof(msg), "End of file before end of string");
                lex->tok.position = lex->pos;
                error_print(lex, msg); 
                exit(1);
                return false;
            }
            /* consume backslash */
            lex->pos++;
            
            /* get escaped char if present */
            c = lex->source[lex->pos];
            
            /* optional: validate escape characters */
            if (strchr("ntr\\\"0", c) == NULL) {
                char msg[80];
                snprintf(msg, sizeof(msg), "Undefined escape character: %c", c);
                lex->tok.position = lex->pos;
                error_print(lex, msg); 
                exit(1);
                return false;
            }
            lex->pos++;
            continue;
        }
        
        if (c == '"') {
            lex->pos++;
            return true;
        }
        
        lex->pos++;
    }
    
    char msg[64];
    snprintf(msg, sizeof(msg), "End of file before end of string");
    lex->tok.position = lex->pos;
    error_print(lex, msg); 
    exit(1);
    return false;
}

bool good_comment(Lexer* lex) {
    //on vient de lire le * du /*
    advance(lex);
        const char *end = strstr(lex->source + lex->pos, "*/");  // Cherche la fin

        if (!end) {
            char msg[46];
            sprintf(msg, "End of file before end of comment");
            error_print(lex, msg); exit(1);
            return false;
        } else {

            lex->pos = (end + 2 - lex->source); // pos arrive juste après le /
            return true;
        }
    
}

void find_next_newline_or_eof(Lexer *lex) {
    //lex->pos se trouve juste après le //
    while (lex->source[++lex->pos] != '\0') {     // Parcourt jusqu'à la fin du code source
        if (lex->source[lex->pos] == '\n')      // Trouve un saut de ligne ?
            break;           
    }
}