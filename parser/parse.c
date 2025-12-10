#include "parse.h"
#include "prod_table.h"
#include "parseerrors.h"

ASTStack* astk;
Stack* stk;

Production prod_table[RULE_SIZE][TOKENTYPE_SIZE];

AST* parse_main(Lexer* lex, int debug)
{
	init_prod_table();
	init_err_table();

	stk = stack_init();
	astk = ast_stack_init(); 	// the astk is global now deal wif it ...

	stack_push(stk, (NUT){.type = RULE, .rule = FIL});


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
					prod.action(lex);
				}
				break;
			}
			case TOKEN: {
				if (curr.token == TOKEN_EOF && curr.token == current_token.type)
					break;

				if (curr.token != current_token.type) {
					error_under_token(lex->source, current_token, "Unexpected token");
					exit(1);
				}
				lexer_next(lex);

				break;
			}
		}
	}

	// (remove this comment when done pls) TODO: remove after actions are implemented
	// if (astk->top != 1) {
	// 	parser_error("Parse finished with %lu AST nodes (expected 1)", astk->top);
	// 	exit(1);
	// }

	AST* result = ast_stack_pop(astk);
	free(stk); 
	free(astk);
	return result;
}
