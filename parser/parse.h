#ifndef RECURSIVE_H
#define RECURSIVE_H

#include <lexer/lexer.h>
#include <ast/ast.h>
#include <error/print_error.h>


/**
 * biteflags for the type of errors and the synchronation token(s) associated with it
 */
typedef enum {
	STMT_ERROR			= 1 << TOKEN_PTSVG,
	BODY_ERROR			= 1 << TOKEN_RACC,
	CLASS_BODY_ERROR	= 1 << TOKEN_RACC | 1 << TOKEN_PTSVG,
	CLASS_ERROR			= 1 << TOKEN_CLASS
} error_type;

File* parse_file(Lexer* lex);
bool iserr(Token t);
Token match(Lexer* lex, TokenType* values);
Token match_nofail(Lexer* lex, TokenType* values, const char* msg);
Type* parse_type(Lexer* lex);
void parse_cro_opt(Lexer* lex);
bool parse_public_opt(Lexer* lex);
bool parse_static_opt(Lexer* lex);
Token parse_extends_opt(Lexer* lex);
ClassDecl* parse_class(Lexer* lex);
void parse_decl_list(Lexer* lex, ClassDecl* class);

#endif
