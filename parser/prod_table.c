#include <parser/prod_table.h>

void init_prod_table(void)
{
	// productions ADD
    prod_table[ADD][TOKEN_BINOP_ADD] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_OPP]       = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_NUMBER]    = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_TRUE]      = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_FALSE]     = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions ADD_SUITE
    prod_table[ADD_SUITE][TOKEN_BINOP_ADD] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_ADD, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_binop_create};
    prod_table[ADD_SUITE][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUB, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_binop_create};
    prod_table[ADD_SUITE][TOKEN_EOF]       = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};

	// productions MULT
    prod_table[MULT][TOKEN_BINOP_ADD] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_binop_create};
    prod_table[MULT][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_binop_create};
    prod_table[MULT][TOKEN_OPP]       = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_binop_create};
    prod_table[MULT][TOKEN_NUMBER]    = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_TRUE]      = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_FALSE]     = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions MULT_SUITE
    prod_table[MULT_SUITE][TOKEN_BINOP_ADD]  = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};
    prod_table[MULT_SUITE][TOKEN_BINOP_SUB]  = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};
    prod_table[MULT_SUITE][TOKEN_BINOP_MULT] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MULT, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_binop_create};
    prod_table[MULT_SUITE][TOKEN_BINOP_DIV]  = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_DIV, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_binop_create};
    prod_table[MULT_SUITE][TOKEN_BINOP_MOD]  = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MOD, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_binop_create};
    prod_table[MULT_SUITE][TOKEN_EOF]        = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};

	// productions UNARY
    prod_table[UNARY][TOKEN_BINOP_ADD] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_ADD, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_unop_create};
    prod_table[UNARY][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUB, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_unop_create};
    prod_table[UNARY][TOKEN_OPP]       = (Production) {.exp = {.items = {{.token = TOKEN_OPP, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_unop_create};
    prod_table[UNARY][TOKEN_NUMBER]    = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_TRUE]      = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_FALSE]     = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions C_O_P
    prod_table[C_O_P][TOKEN_NUMBER]= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_TRUE]  = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions POSTFIX_EXPR
    prod_table[POSTFIX_EXPR][TOKEN_NUMBER]= (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_TRUE]  = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions POSTFIX_TAIL
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_ADD]  = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_SUB]  = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_MULT] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_DIV]  = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_MOD]  = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};
    prod_table[POSTFIX_TAIL][TOKEN_EOF]        = (Production) {.exp = {.items = {}, .count = 0}, .action = action_binop_end};

	// productions PRIMARY
    prod_table[PRIMARY][TOKEN_NUMBER]= (Production) {.exp = {.items = {{.token = TOKEN_NUMBER, .type = TOKEN}}, .count = 1}, .action = action_create_number_leaf};
    prod_table[PRIMARY][TOKEN_TRUE]  = (Production) {.exp = {.items = {{.token = TOKEN_TRUE, .type = TOKEN}}, .count = 1}, .action = action_create_boolean_leaf};
    prod_table[PRIMARY][TOKEN_FALSE] = (Production) {.exp = {.items = {{.token = TOKEN_FALSE, .type = TOKEN}}, .count = 1}, .action = action_create_boolean_leaf};
}
