#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


#include <lexer/lexer.h>
#include <lexer/string_lex.h>
#include <error/print_error.h>


bool good_string(Lexer* lex) {
    //on vient de lire " dans le lexeur


    advance(lex);
    char c; 

    while(lex->pos < lex->length) {
        c = peek(lex);
        if (c == '\\') {
            if (lex->pos == lex->length - 1) {
                char msg[46];
                sprintf(msg, "End of file before end of string");
                error_print(lex, msg);
                return false;
            }

            advance(lex); 
            c = peek(lex);
            if (strchr("n\\\"r\'0", c) == NULL) {

                //c n'est pas dans [n, \, ", ', 0]
                char msg[46];
                sprintf(msg, "Undefined escape character : %c \n", c);
                error_print(lex, msg);
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
    char msg[46];
    sprintf(msg, "End of file before end of string");
    error_print(lex, msg);
    lex->tok.position = lex->pos;


    return false;
}

bool good_comment(Lexer* lex) {
    //on vient de lire le * du /*
    advance(lex);
        const char *end = strstr(lex->source + lex->pos, "*/");  // Cherche la fin

        if (!end) {
            char msg[46];
            sprintf(msg, "End of file before end of comment");
            error_print(lex, msg);
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