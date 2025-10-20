#ifndef PARSE_H
#define PARSE_H

#include "backend_parser.h"

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
AST* parse_main(Lexer* lex);

#endif
