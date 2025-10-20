#ifndef PARSE_H
#define PARSE_H

#include "lexer/lexer.h"
#include "ast/ast.h"

/**
 * implémentation pour la sous-grammaire:
	add -> mult add_suite .
	add_suite -> "+" mult add_suite .
	add_suite -> "-" mult add_suite .
	add_suite -> .

	mult -> unary mult_suite .
	mult_suite -> "*" unary mult_suite .
	mult_suite -> "/" unary mult_suite .
	mult_suite -> "%" unary mult_suite .
	mult_suite -> .

	unary -> "!" unary .
	unary -> "-" unary .
	unary -> "+" unary .
	unary -> cop .

	cop -> postfix_expr .
	postfix_expr -> primary postfix_tail .
	postfix_tail -> .

	primary -> int .
	primary -> true .
	primary -> false .

	(renommée à partir de celle d'Hugo (celle qui est partagée dans le drive))
 */

#define PARSER_STACK_SIZE 256

typedef enum {
	ADD,
	ADD_SUITE,
	MULT,
	MULT_SUITE,
	UNARY,
	C_O_P,
	POSTFIX_EXPR,
	POSTFIX_TAIL,
	PRIMARY
} Rule;

// Node in the parser stack: either a rule (non-terminal) or token (terminal)
typedef struct {
	enum {RULE, TOKEN} type;
	union {
		Token token;
		Rule rule;
	};
} NUT;

typedef struct {
	NUT items[PARSER_STACK_SIZE];
	int top;
} Stack;

typedef struct {
	AST* items[PARSER_STACK_SIZE];
    int top;
} ASTStack;

AST* parse_main(Lexer* lex);

#endif
