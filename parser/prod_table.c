#include "prod_table.h"

void init_prod_table(void) {
	// ADD productions
	prod_table[ADD][TOKEN_BINOP_ADD]	= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = NULL};
	prod_table[ADD][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = NULL};
	prod_table[ADD][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = NULL};
	prod_table[ADD][TOKEN_TRUE]			= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = NULL};
	prod_table[ADD][TOKEN_FALSE]		= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = NULL};
	prod_table[ADD][TOKEN_OPP]			= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = NULL};

	// ADD_SUITE productions
	prod_table[ADD_SUITE][TOKEN_BINOP_ADD]	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_ADD, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUB, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_EOF]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_suite_binop};

	// MULT productions
	prod_table[MULT][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_FALSE]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_OPP]			= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_BINOP_ADD]	= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// MULT_SUITE productions
	prod_table[MULT_SUITE][TOKEN_BINOP_MULT]	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MULT, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_DIV]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_DIV, .type = TOKEN},  {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_MOD]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MOD, .type = TOKEN},  {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_ADD]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_SUB]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_EOF]			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// UNARY productions
	prod_table[UNARY][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_FALSE]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_BINOP_ADD]	= (Production) {.exp = {.items = {{.token = TOKEN_UNOP_ADD, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[UNARY][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.token = TOKEN_UNOP_SUB, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[UNARY][TOKEN_OPP]		= (Production) {.exp = {.items = {{.token = TOKEN_OPP, .type = TOKEN},  	{.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_stub};

	// C_O_P productions
	prod_table[C_O_P][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = NULL};
	prod_table[C_O_P][TOKEN_TRUE]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = NULL};
	prod_table[C_O_P][TOKEN_FALSE]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = NULL};

	// POSTFIX_EXPR productions
	prod_table[POSTFIX_EXPR][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = NULL};
	prod_table[POSTFIX_EXPR][TOKEN_TRUE]	= (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = NULL};
	prod_table[POSTFIX_EXPR][TOKEN_FALSE]	= (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = NULL};

	// POSTFIX_TAIL productions
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_MULT]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_suite_unop};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_DIV]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_suite_unop};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_MOD]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_suite_unop};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_ADD]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_suite_unop};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_suite_unop};
	prod_table[POSTFIX_TAIL][TOKEN_EOF]			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_suite_unop};
	
	// PRIMARY productions
	prod_table[PRIMARY][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.token = TOKEN_NUMBER,.type = TOKEN}}, .count = 1}, .action = NULL};
	prod_table[PRIMARY][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.token = TOKEN_TRUE,  .type = TOKEN}}, .count = 1}, .action = NULL};
	prod_table[PRIMARY][TOKEN_FALSE]	= (Production) {.exp = {.items = {{.token = TOKEN_FALSE, .type = TOKEN}}, .count = 1}, .action = NULL};
}
