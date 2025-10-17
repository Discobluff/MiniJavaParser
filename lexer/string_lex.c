#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "lexer.h"
#include "string_lex.h"

bool good_string(Lexer* lex) {
    //on vient de lire " dans le lexeur
    advance(lex);
    char c; 

    while(lex->pos < lex->length) {
        c = peek(lex);
        if (c == '\\') {
            if (lex->pos == lex->length - 1) {
                printf("Syntax error : end of file before end of string at pos %d\n", lex->pos);
                return false;
            }

            advance(lex); 
            c = peek(lex);
            if (strchr("n\\\"r\'0", c) == NULL) {
                //c n'est aucun de ces caractères
                printf("Syntax error : undefined escape character : %c \n", c);
                return false;
            }
            
        }

        // on n'a pas lu antislash
        if (c == '\"') {
            advance(lex);
            return true;
        }

        else { advance(lex); }


    }
    printf("Syntax Error : end of file before end of string pos %d\n", lex->pos);
    lex->tok.position = lex->pos;
    return false;
}