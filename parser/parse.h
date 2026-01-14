#ifndef RECURSIVE_H
#define RECURSIVE_H

#include <lexer/lexer.h>
#include <ast/ast.h>
#include <error/print_error.h>


File* parse_file(Lexer* lex);
bool iserr(Token t);
Token match(Lexer* lex, TokenType* values);
Token match_nofail(Lexer* lex, TokenType* values, const char* msg, TokenType* error);
Type* parse_type(Lexer* lex);
void parse_cro_opt(Lexer* lex);
bool parse_public_opt(Lexer* lex);
bool parse_static_opt(Lexer* lex);
Token parse_extends_opt(Lexer* lex);
ClassDecl* parse_class(Lexer* lex);
void parse_decl_list(Lexer* lex, ClassDecl* class);

#endif
