#ifndef STR_LEX_H
#define STR_LEX_H

#include "lexer.h"

bool good_string(Lexer* lex);
bool good_comment(Lexer* lex);
void find_next_newline_or_eof(Lexer *lex);

#endif