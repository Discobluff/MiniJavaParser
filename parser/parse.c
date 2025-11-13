#include "error/print_error.h"
#include <stdio.h>
#include <stdbool.h>
#include "backend_parser.h"
#include "parse.h"


static const RuleSpec RULE_SPECS[] =
{
	[ADD]			= {0, "addition expression"},
	[ADD_SUITE]		= {3, "addition continuation"},
	[MULT]			= {0, "multiplication expression"},
	[MULT_SUITE]	= {3, "multiplication continuation"},
	[UNARY]			= {1, "unary expression"},
	[C_O_P]			= {0, "call or primary"},
	[POSTFIX_EXPR]	= {0, "postfix expression"},
	[POSTFIX_TAIL]	= {0, "postfix tail"},
	[PRIMARY]		= {0, "primary expression"}
};

// has holes if done like above (idk what would happen if we would access holes)
/**
 * This answers the question: when do I stop chaining the _suite operations
 * -> when we see a char that separates two same _suite we don't reduce yet.
 */
static const size_t REDUCTION_TABLE[] =
{
	[ADD] 			= FOLLOW_EOF,
	[ADD_SUITE] 	= FOLLOW_ALL & ~FOLLOW_ADDOP,	// Reduce unless we see + or -
	[MULT_SUITE] 	= FOLLOW_ALL & ~FOLLOW_MULTOP,	// Reduce unless we see * / %
	[POSTFIX_TAIL] 	= FOLLOW_ALL,					// Always reduce
	[UNARY] 		= FOLLOW_NONE,					// Never reduce on lookahead
	[C_O_P] 		= FOLLOW_NONE,
	[POSTFIX_EXPR] 	= FOLLOW_NONE,
	[PRIMARY] 		= FOLLOW_NONE,
	[MULT] 			= FOLLOW_ALL & ~FOLLOW_MULTOP & ~FOLLOW_ADDOP & ~FOLLOW_UNOP,
};


static size_t classify_token(TokenType type)
{
	switch (type) {
		case TOKEN_BINOP_ADD:
		case TOKEN_BINOP_SUB:
			return FOLLOW_ADDOP;
		
		case TOKEN_BINOP_MULT:
		case TOKEN_BINOP_DIV:
		case TOKEN_BINOP_MOD:
			return FOLLOW_MULTOP;
		
		case TOKEN_EOF:
			return FOLLOW_EOF;

		case TOKEN_OPP:
			return FOLLOW_UNOP;

		default:
			return FOLLOW_NONE;
	}
}

static bool should_reduce(Rule rule, Token lookahead)
{
	size_t lookahead_class = classify_token(lookahead.type);
	return (REDUCTION_TABLE[rule] & lookahead_class) != 0;
}

static bool is_primary_token(TokenType type)
{
	return	type == TOKEN_NUMBER
			|| type == TOKEN_TRUE
			|| type == TOKEN_FALSE;
}

static bool is_binary_prefix(TokenType type)
{
	return	type == TOKEN_BINOP_ADD
			|| type == TOKEN_BINOP_SUB
			|| type == TOKEN_BINOP_MULT
			|| type == TOKEN_BINOP_DIV
			|| type == TOKEN_BINOP_MOD;
}

// static bool is_unary_prefix(TokenType type)
// {
// 	return	type == TOKEN_UNOP_ADD
// 			|| type == TOKEN_UNOP_SUB
// 			|| type == TOKEN_OPP;
// }

// Normalise the token to disambiguate it
static Token normalize_unary_token(Token t)
{
	if (t.type == TOKEN_BINOP_SUB || t.type == TOKEN_BINOP_ADD) {
		return (Token){
			.type = (t.type == TOKEN_BINOP_SUB) ? TOKEN_UNOP_SUB : TOKEN_UNOP_ADD,
			.position = t.position
		};
	}
	return t;
}

static Expansion expand_rule(Rule rule, Token lookahead)
{
	Expansion exp = {0};

	switch (rule) {
		case ADD:
			if (is_primary_token(lookahead.type) || is_binary_prefix(lookahead.type) || lookahead.type == TOKEN_OPP) {
				exp.items[0] = (NUT){.rule = ADD_SUITE, .type = RULE};
				exp.items[1] = (NUT){.rule = MULT, .type = RULE};
				exp.count = 2;
			}
			break;
			
		case ADD_SUITE:
			if (lookahead.type == TOKEN_BINOP_ADD || lookahead.type == TOKEN_BINOP_SUB) {
				exp.items[0] = (NUT){.rule = ADD_SUITE, .type = RULE};
				exp.items[1] = (NUT){.rule = MULT, .type = RULE};
				exp.items[2] = (NUT){.token = lookahead, .type = TOKEN};
				exp.count = 3;
			} else if (lookahead.type == TOKEN_EOF) {
				exp.count = 0; // Epsilon production
			}
			break;
			
		case MULT:
			if (is_binary_prefix(lookahead.type) || is_primary_token(lookahead.type) || lookahead.type == TOKEN_OPP) {
				exp.items[0] = (NUT){.rule = MULT_SUITE, .type = RULE};
				exp.items[1] = (NUT){.rule = UNARY, .type = RULE};
				exp.count = 2;
			}
			break;
			
		case MULT_SUITE:
			switch (lookahead.type) {
				case TOKEN_BINOP_MULT:
				case TOKEN_BINOP_DIV:
				case TOKEN_BINOP_MOD:
					exp.items[0] = (NUT){.rule = MULT_SUITE, .type = RULE};
					exp.items[1] = (NUT){.rule = UNARY, .type = RULE};
					exp.items[2] = (NUT){.token = lookahead, .type = TOKEN};
					exp.count = 3;
					break;
				case TOKEN_BINOP_ADD:
				case TOKEN_BINOP_SUB:
				default:
					exp.count = 0; // Epsilon
					break;
			}
			break;
			
		case UNARY:
			if (is_binary_prefix(lookahead.type) || lookahead.type == TOKEN_OPP) { // Lexer outputs only "BINOP_SUB" for '-' char
				exp.items[0] = (NUT){.rule = UNARY, .type = RULE};
				exp.items[1] = (NUT){.token = normalize_unary_token(lookahead), .type = TOKEN};
				exp.count = 2;
			} else if (is_primary_token(lookahead.type)) {
				exp.items[0] = (NUT){.rule = C_O_P, .type = RULE};
				exp.count = 1;
			}
			break;
			
		case C_O_P:
			if (is_primary_token(lookahead.type)) {
				exp.items[0] = (NUT){.rule = POSTFIX_EXPR, .type = RULE};
				exp.count = 1;
			}
			break;
			
		case POSTFIX_EXPR:
			if (is_primary_token(lookahead.type)) {
				exp.items[0] = (NUT){.rule = POSTFIX_TAIL, .type = RULE};
				exp.items[1] = (NUT){.rule = PRIMARY, .type = RULE};
				exp.count = 2;
			}
			break;
			
		case POSTFIX_TAIL:
			// Always epsilon
			exp.count = 0;
			break;
			
		case PRIMARY:
			if (is_primary_token(lookahead.type)) {
				exp.items[0] = (NUT){.token = lookahead, .type = TOKEN};
				exp.count = 1;
			}
			break;
	}

	return exp;
}

static void rule_handler(Stack* stk, Rule rule, Lexer* lex)
{
	Expansion exp = expand_rule(rule, lex->tok);

	if (exp.count < 0) {
		error_under_token(lex->source, lex->tok, 
			"Unexpected token in %s", RULE_SPECS[rule].name);
		exit(1);
	}


	for (int i = 0; i < exp.count; i++) {
		stack_push(stk, exp.items[i]);
	}

	return;
}

// Uses a token from the semantic stack (not from the lexer) they could be different (cf unary_op)
static AST* create_token_ast(Token token, Lexer* lex)
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

// Reduces the AST to the binary operation and pushes it back in
static void reduce_binary(ASTStack* ast_stk, const char* rule_name)
{
	if (ast_stk->top < 3) {
		return ; // cannot reduce 
	}

	AST* right = ast_stack_pop(ast_stk);
	AST* acc = ast_stack_pop(ast_stk);
	AST* left = ast_stack_pop(ast_stk);

	if (!acc || acc->type != BINARY_OP) {
		parser_error("Expected binary operator in %s reduction", rule_name);
		exit(1);
	}

	while (acc && acc->type == BINARY_OP) {
		acc->data.binary.left = left;
		acc->data.binary.right = right;
		
		// Current acc is complete, make it the right child of the next operator
		right = acc;
		
		if (ast_stk->top >= 2) {  // Need at least operator + left operand
			acc = ast_stack_pop(ast_stk);
			left = ast_stack_pop(ast_stk);
		} else {
			break;  // No more operators to chain
		}
	}
	ast_stack_push(ast_stk, acc);
	return ;
}

static void reduce_unary(ASTStack* ast_stk)
{
	if (ast_stk->top < 1)
		return ;

	AST* operand = ast_stack_pop(ast_stk);

	if (ast_stk->top > 0) {
		AST* top = ast_stack_peek(ast_stk);
		
		while (top && top->type == UNARY_OP) {
			AST* op = ast_stack_pop(ast_stk);
			op->data.unary.operand = operand;
			operand = op;
			
			if (ast_stk->top > 0) {
				top = ast_stack_peek(ast_stk);
			} else {
				break;
			}
		}
	}

	ast_stack_push(ast_stk, operand);
}

// reduces the rule and pushes it into the AST stack
static void reduce_rule(Rule rule, ASTStack* ast_stk)
{
	const RuleSpec* spec = &RULE_SPECS[rule];
	// printf("reducing %s\n", spec->name); // print_ast_parser(ast_stk); printf("\n");

	if (ast_stk->top < spec->min_ast_nodes) {
		// parser_error("Cannot reduce %s: need %zu AST nodes, have %zu",
		// 			spec->name, spec->min_ast_nodes, ast_stk->top);
		return ;
	}

	switch (rule) {
		case ADD_SUITE:
			reduce_binary(ast_stk, RULE_SPECS[rule].name);
			break ;
			
		case MULT_SUITE:
			reduce_binary(ast_stk, RULE_SPECS[rule].name);
			break ;
			
		case UNARY:
			break ;
			
		case POSTFIX_EXPR: 
		case C_O_P:
		case PRIMARY:
			// ast_stack_pop(ast_stk);
		break;
		
		case POSTFIX_TAIL:
			reduce_unary(ast_stk);
			break;
			
		case ADD:
		case MULT:
			// ast_stack_pop(ast_stk);
			break;

		default:
			parser_error("Unknown rule in reduce: %s", RULE_SPECS[rule].name);
			exit(1);
	}
}

AST* parse_main(Lexer* lex, int debug)
{
	Stack* stk = stack_init();
	ASTStack* astk = ast_stack_init();

	// stack_push(stk, (NUT){.type = TOKEN, .token = {.position=0, .type=TOKEN_EOF}});
	stack_push(stk, (NUT){.type = RULE, .rule = ADD});

	while (stk->top > 0) {
		if (debug) {
			printf("==================== NEW ITERATION ===================\n"); 
			print_parser(stk);
			printf("\n");
			token_pprint(lex->tok, lex);
			printf("\n");
		}
		NUT curr = stack_pop(stk);

		switch (curr.type) {
			case RULE: {
				if (should_reduce(curr.rule, lex->tok)) {
					if (debug) {
						printf("reducing on AST stack\n"); print_ast_parser(astk); printf("\n");
					}
					reduce_rule(curr.rule, astk);
					if (debug) {
						printf("produced stack\n"); 
						print_ast_parser(astk); 
						printf("\n");
					}
				} else {
					rule_handler(stk, curr.rule, lex);
				}
				break;
			}
			case TOKEN: {
				AST* node = create_token_ast(curr.token, lex);
				if (node)
					ast_stack_push(astk, node);
				lexer_next(lex);
				break;
			}
		}
	}

	if (astk->top != 1) {
		parser_error("Parse finished with %lu AST nodes (expected 1)", astk->top);
		exit(1);
	}

	AST* result = ast_stack_pop(astk);

	free(stk); free(astk);
	return result;
}
