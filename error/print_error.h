#ifndef PRINT_ERROR_H
#define  PRINT_ERROR_H

#include "lexer/lexer.h"

void error_print(Lexer* lex, const char* message);
void error_under_token(char* src, Token tok, const char* message);

#endif