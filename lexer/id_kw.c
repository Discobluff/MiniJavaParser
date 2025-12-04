#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "lexer.h"
#include "id_kw.h"


Token id_kw_class (Lexer* lex, int start) {
    //this function is called afer having read an alpha
    char c = peek(lex);
    char content[32]; //maximum 31 caractères
    int index= 0;
    Token tok;
    while(lex->pos < lex->length) {

        if(isalpha(c) || c == '_' || isdigit(c)) {
            content[index++] = c;
            
            advance(lex);
            c = peek(lex);
            continue;
        }

        else {
            break;
            }
        }
    
    content[index] = '\0';
    tok.position = start;

    if (strcmp(content, "class") == 0) {
        tok.type = TOKEN_CLASS;
    }
    else {
        tok.type = get_keyword_token(content);
        if (tok.type == TOKEN_ID) {
            // check if it's a declared type
            for (int i=0; i<lex->decl_type_list_size; i++) {
                char content1[32]; char content2[32];
                token_identifier(lex, tok, content1, 32);
                token_identifier(lex, (Token){.type = TOKEN_ID_TYPE, .position = lex->decl_type_list[i]}, content2, 32);
                printf("Comparing %s and %s %d\n", content1, content2, i);
                if (strcmp(content1, content2) == 0) {
                    tok.type = TOKEN_ID_TYPE;
                    return tok;
                }
            }
        }
    }
    return tok;
    
}
    
    

TokenType get_keyword_token(const char *keyword) {
    if (strcmp(keyword, "boolean") == 0) {
        return TOKEN_BOOLEAN;
    } else if (strcmp(keyword, "else") == 0) {
        return TOKEN_ELSE;
    } else if (strcmp(keyword, "extends") == 0) {
        return TOKEN_EXTENDS;
    } else if (strcmp(keyword, "false") == 0) {
        return TOKEN_FALSE;
    } else if (strcmp(keyword, "for") == 0) {
        return TOKEN_FOR;
    } else if (strcmp(keyword, "if") == 0) {
        return TOKEN_IF;
    } else if (strcmp(keyword, "instanceof") == 0) {
        return TOKEN_INSTANCEOF;
    } else if (strcmp(keyword, "int") == 0) {
        return TOKEN_INT;
    } else if (strcmp(keyword, "new") == 0) {
        return TOKEN_NEW;
    } else if (strcmp(keyword, "null") == 0) {
        return TOKEN_NULL;
    } else if (strcmp(keyword, "public") == 0) {
        return TOKEN_PUBLIC;
    } else if (strcmp(keyword, "return") == 0) {
        return TOKEN_RETURN;
    } else if (strcmp(keyword, "static") == 0) {
        return TOKEN_STATIC;
    } else if (strcmp(keyword, "this") == 0) {
        return TOKEN_THIS;
    } else if (strcmp(keyword, "true") == 0) {
        return TOKEN_TRUE;
    } else if (strcmp(keyword, "void") == 0) {
        return TOKEN_VOID;
    } else {
        return TOKEN_ID;
    }
}

