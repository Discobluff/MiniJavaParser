/**
 * electric boogaloo
 * https://www.youtube.com/watch?v=Zo8v7FrM3mI
 * https://www.youtube.com/watch?v=B7nKzCRL_oo
 */

#include "parse.h"
#include "prod_table.h"
#include "parseerrors.h"

ASTStack* astk;
Stack* stk;

Production prod_table[RULE_SIZE][TOKENTYPE_SIZE];

// table of Productions indexed by non-terminals (Rule) and tokens (TokenType) santa maria madre de dios


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
	init_err_table();

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
					print_syntax_error(lex, curr.rule, current_token.type);
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
