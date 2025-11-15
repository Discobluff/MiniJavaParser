/**
 * electric boogaloo
 * https://www.youtube.com/watch?v=Zo8v7FrM3mI
 * https://www.youtube.com/watch?v=B7nKzCRL_oo
 */

#include "parse2.h"


ASTStack* astk;
Stack* stk;


static void action_suite_binop(void)
{
	if (astk->top < 3) {
		return ; // cannot reduce 
	}

	AST* right = ast_stack_pop(astk);
	AST* acc = ast_stack_pop(astk);
	AST* left = ast_stack_pop(astk);


	while (acc && acc->type == BINARY_OP) {
		acc->data.binary.left = left;
		acc->data.binary.right = right;
		
		right = acc;
		
		if (astk->top >= 2) {
			acc = ast_stack_pop(astk);
			left = ast_stack_pop(astk);
		} else {
			break ;
		}
	}
	ast_stack_push(astk, acc);
	return ;
}

static void action_suite_unop(void)
{
	if (astk->top < 1)
		return ;

	AST* operand = ast_stack_pop(astk);

	if (astk->top > 0) {
		AST* top = ast_stack_peek(astk);
		
		while (top && top->type == UNARY_OP) {
			AST* op = ast_stack_pop(astk);
			op->data.unary.operand = operand;
			operand = op;
			
			if (astk->top > 0) {
				top = ast_stack_peek(astk);
			} else {
				break ;
			}
		}
	}

	ast_stack_push(astk, operand);
	return ;
}

// what the sigma? used for not having NULL as the action function (so we know when we have an error in the main loop of the parser)
static void action_stub(void) { return ; }

static Production prod_table[RULE_SIZE][TOKENTYPE_SIZE];

// table of Productions indexed by non-terminals (Rule) and tokens (TokenType) santa maria madre de dios
static void init_prod_table(void)
{
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

static AST* create_ast_token(Token token, Lexer* lex)
{
	// printf("Creating AST for token type: %d\n", token.type);

	switch (token.type) {
		case TOKEN_NUMBER:
			return create_number_node(token_number(lex, token));
		case TOKEN_TRUE:
			return create_number_node(1);
		case TOKEN_FALSE:
			return create_number_node(0);
		case TOKEN_BINOP_ADD:
			return create_binary_op_node('+', NULL, NULL);
		case TOKEN_BINOP_SUB:
			return create_binary_op_node('-', NULL, NULL);
		case TOKEN_BINOP_MULT:
			return create_binary_op_node('*', NULL, NULL);
		case TOKEN_BINOP_DIV:
			return create_binary_op_node('/', NULL, NULL);
		case TOKEN_BINOP_MOD:
			return create_binary_op_node('%', NULL, NULL);
		case TOKEN_OPP:
			return create_unary_op_node('!', NULL);
		case TOKEN_UNOP_ADD:
			return create_unary_op_node('+', NULL);
		case TOKEN_UNOP_SUB:
			return create_unary_op_node('-', NULL);
		default:
			return NULL;
	}
}


AST* parse_main(Lexer* lex, int debug)
{
	init_prod_table();

	stk = stack_init();
	astk = ast_stack_init(); 	// the astk is global now deal wif it ...

	stack_push(stk, (NUT){.type = RULE, .rule = ADD});


	while (stk->top > 0) {
		if (debug) {
			printf("==================== NEW ITERATION ===================\n"); 
			print_parser(stk);
			printf("\n");
			token_pprint(lex->tok, lex);
			printf("\n");
			print_ast_parser(astk);
			printf("\n");
		}
		
		NUT curr = stack_pop(stk);
		Token current_token = lexer_giveToken(lex);

		switch (curr.type) {
			case RULE: {
				Production prod = prod_table[curr.rule][current_token.type];
				
				// Check if production exists (internal error)
				if (prod.exp.count == 0 && prod.action == NULL) {
					parser_error("No production for rule %d and token %d", curr.rule, current_token.type);
					exit(1);
				}

				for (int i = prod.exp.count - 1; i >= 0; i--) {
					stack_push(stk, prod.exp.items[i]);
				}

				if (prod.action) {
					prod.action();
				}
				break;
			}
			case TOKEN: {
				if (curr.token == TOKEN_EOF && curr.token == current_token.type)
					break;

				if (token_type_equality(curr.token, current_token)) { 
					AST* token_node = create_ast_token((Token){.position = current_token.position, .type = curr.token}, lex);
					if (token_node) {
						ast_stack_push(astk, token_node);
					}
					lexer_next(lex);
				} else {
					parser_error("Expected token %d, got %d", curr.token, current_token.type);
					exit(1);
				}
				break;
			}
		}
	}

	if (astk->top != 1) {
		parser_error("Parse finished with %lu AST nodes (expected 1)", astk->top);
		exit(1);
	}

	AST* result = ast_stack_pop(astk);
	free(stk); 
	free(astk);
	return result;
}
