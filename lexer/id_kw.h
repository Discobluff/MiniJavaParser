
#ifndef ID_H
#define ID_H

#include <lexer/lexer.h>


TokenType get_keyword_token(const char *keyword);
Token id_kw_class (Lexer* lex, int start);
#endif