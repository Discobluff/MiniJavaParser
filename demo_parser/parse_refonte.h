#ifndef PARSE_REFONTE_H
#define PARSE_REFONTE_H

#include "backend_parser.h"

#define MAXIMUM_EXPANSION 4

typedef struct {
	size_t min_ast_nodes;	// Minimum AST nodes needed for reduction
	const char* name;		// For debugging/error messages
} RuleSpec;

#define FOLLOW_NONE		(0)
#define FOLLOW_ADDOP	(1 << 0)	// + -
#define FOLLOW_MULTOP	(1 << 1)	// * / %
#define FOLLOW_EOF		(1 << 3)
#define FOLLOW_UNOP		(1 << 4)
#define FOLLOW_ALL		(FOLLOW_ADDOP | FOLLOW_MULTOP | FOLLOW_EOF | FOLLOW_UNOP)

typedef struct {
	Rule rule;
	size_t follow_set;  // Reduce when lookahead is in this set
} ReductionRule;

typedef struct {
	NUT items[MAXIMUM_EXPANSION];  // Maximum expansion is 3 items + sentinel
	int count;
} Expansion;

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
AST* parse_refonte_main(Lexer* lex);

#endif
