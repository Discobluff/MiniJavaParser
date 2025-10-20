#include "parse.h"
#include "error/print_error.h"
// #include <string.h>
#include <stdio.h>
#include <stdbool.h>

// ============================ BACKEND FUNCTIONS =====================================


Stack* stack_init()
{
	Stack* stk = malloc(sizeof(Stack));
	stk->top = 0;
	return stk;
}

void stack_push(Stack* stk, NUT value)
{
	if(stk->top < PARSER_STACK_SIZE) {
		stk->items[stk->top] = value;
		stk->top++;
	}
	else {
		parser_error("PARSER INTERNAL STACK OVERFLOW!");
		exit(1);
	}
}

NUT stack_pop(Stack* stk)
{
	if(stk->top > 0)
		return stk->items[--stk->top];
	else {
		parser_error("PARSER INTERNAL STACK UNDERFLOW!");
		exit(1);
	}
}

void print_parser(Stack* stk)
{
	printf("Parser Stack (top -> bottom):\n");
	for (int i = stk->top - 1; i >= 0; i--) {
		NUT nut = stk->items[i];
		if (nut.type == RULE) {
			printf("  [RULE] %d\n", nut.rule);
		} else {
			printf("  [TOKEN] %s\n", token_type_name(nut.token.type));
		}
	}
}

ASTStack* ast_stack_init()
{
	ASTStack* astk = malloc(sizeof(ASTStack));
	astk->top = 0;
	return astk;
}

void ast_stack_push(ASTStack* stk, AST* value)
{
	if(stk->top < PARSER_STACK_SIZE) {
		stk->items[stk->top] = value;
		stk->top++;
	}
	else {
		parser_error("PARSER INTERNAL AST STACK OVERFLOW!");
		exit(1);
	}
}

AST* ast_stack_pop(ASTStack* stk)
{
	if(stk->top > 0)
		return stk->items[--stk->top];
	else {
		parser_error("PARSER INTERNAL AST STACK UNDERFLOW!");
		exit(1);
	}
}

AST* ast_stack_peek(ASTStack* stk)
{
	if (stk->top > 0)
		return stk->items[stk->top - 1];
	return NULL;
}

void print_ast_parser(ASTStack* stk)
{
	printf("Parser Stack (top -> bottom):\n");
	for (int i = stk->top - 1; i >= 0; i--) {
		AST* ast = stk->items[i];
		print_ast(ast, 0);
		printf("\n");
	}
	printf("End Parser Stack\n");
}

// ============================ END BACKEND FUNCTIONS =====================================



bool should_reduce(Rule rule, Token lookahead)
{
	switch (rule) {
		case ADD_SUITE:
			switch (lookahead.type) {
				case TOKEN_BINOP_ADD:
				case TOKEN_BINOP_SUB:
					return false;
				default:
					return true;
			}
			
		case MULT_SUITE:
			// Reduces when we see anything that's NOT *, /, %
			switch (lookahead.type) {
				case TOKEN_BINOP_MULT:
				case TOKEN_BINOP_DIV:
				case TOKEN_BINOP_MOD:
					return false;
				default:
					return true;
			}
			
		case POSTFIX_TAIL:
			return true;
			
		case ADD:
		case MULT:
		case UNARY:
		case POSTFIX_EXPR:
		case PRIMARY:
			return false;
		
		case C_O_P:
			return true;

		default:
			return false;
	}
}

Token tmpchange(Token t)
{
	switch (t.type)
	{
	case TOKEN_BINOP_SUB:
		return (Token){.type=TOKEN_BINOP_2EQUAL, .position=t.position};
	case TOKEN_BINOP_ADD:
		return (Token){.type=TOKEN_BINOP_2EQUAL, .position=t.position};
	default:
		return t;
	}
}

// On an already headless stack. Returns if the lexer should move.
bool rule_handler(Stack* stk, Rule rule, Lexer* lex)
{
	bool move_lexer = false;
	switch (rule) {
		case ADD:

			switch (lex->tok.type) {
			case TOKEN_NUMBER:
			case TOKEN_OPP:
			case TOKEN_TRUE:
			case TOKEN_FALSE:
			case TOKEN_BINOP_ADD:
			case TOKEN_BINOP_SUB:
				stack_push(stk, (NUT){.rule = ADD_SUITE, .type = RULE});
				stack_push(stk, (NUT){.rule = MULT, .type = RULE});
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur add");
				exit(1);
			}
			break;

		case ADD_SUITE:
			switch (lex->tok.type){
			case TOKEN_EOF:
				break;
			case TOKEN_BINOP_ADD:
			case TOKEN_BINOP_SUB:
				stack_push(stk, (NUT){.rule = ADD_SUITE, .type = RULE});
				stack_push(stk, (NUT){.rule = MULT, .type = RULE});
				stack_push(stk, (NUT){.token = lex->tok, .type = TOKEN});
				move_lexer = true;
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur add suite");
				exit(1);
			}
			break;

		case MULT_SUITE:
			switch (lex->tok.type){
			case TOKEN_BINOP_ADD:
			case TOKEN_BINOP_SUB:
			case TOKEN_EOF:
				break;
			case TOKEN_BINOP_MULT:
			case TOKEN_BINOP_DIV:
			case TOKEN_BINOP_MOD:
				stack_push(stk, (NUT){.rule = MULT_SUITE, .type = RULE});
				stack_push(stk, (NUT){.rule = UNARY, .type = RULE});
				stack_push(stk, (NUT){.token = lex->tok, .type = TOKEN});
				move_lexer = true;
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur mult suite");
				exit(1);
			}
			break;

		case MULT:
			switch (lex->tok.type){
			case TOKEN_BINOP_SUB:
			case TOKEN_BINOP_ADD:
			case TOKEN_OPP:
			case NUMBER:
			case TOKEN_TRUE:
			case TOKEN_FALSE:
				stack_push(stk, (NUT){.rule = MULT_SUITE, .type = RULE});
				stack_push(stk, (NUT){.rule = UNARY, .type = RULE});
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur mult");
				exit(1);
			}
			break;

		case UNARY:
			switch (lex->tok.type) {
			case TOKEN_BINOP_SUB:
			case TOKEN_BINOP_ADD:
			case TOKEN_OPP:
				stack_push(stk, (NUT){.rule = UNARY, .type = RULE});
				stack_push(stk, (NUT){.token = tmpchange(lex->tok), .type = TOKEN}); // will not work for sub bc of conflict with '-' (unary)
				move_lexer = true;
				break;
			case NUMBER:
			case TOKEN_TRUE:
			case TOKEN_FALSE:
				stack_push(stk, (NUT){.rule = C_O_P, .type = RULE});
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur unary");
				exit(1);
			}
			break;

		case C_O_P:
			switch (lex->tok.type){
			case NUMBER:
			case TOKEN_TRUE:
			case TOKEN_FALSE:
				stack_push(stk, (NUT){.rule = POSTFIX_EXPR, .type = RULE});
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur c o p");
				exit(1);
			}
			break;

		case POSTFIX_EXPR:
			switch (lex->tok.type){
			case NUMBER:
			case TOKEN_TRUE:
			case TOKEN_FALSE:
				stack_push(stk, (NUT){.rule = POSTFIX_TAIL, .type = RULE});
				stack_push(stk, (NUT){.rule = PRIMARY, .type = RULE});
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur post expr");
				exit(1);
			}
			break;

		case POSTFIX_TAIL:
			switch (lex->tok.type){
			case TOKEN_BINOP_ADD:
			case TOKEN_BINOP_SUB:
			case TOKEN_EOF:
			case TOKEN_BINOP_MULT:
			case TOKEN_BINOP_DIV:
			case TOKEN_BINOP_MOD:
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur post tail");
				exit(1);
			}
			break;

		case PRIMARY:
			switch (lex->tok.type){
			case NUMBER:
			case TOKEN_TRUE:
			case TOKEN_FALSE:
				stack_push(stk, (NUT){.token = lex->tok, .type = TOKEN});
				move_lexer = true;
				break;
			default:
				error_under_token(lex->source, lex->tok, "erreur primary");
				exit(1);
			}
			break;

		default:
			parser_error("PARSER INTERNAL UNRECOGNIZED RULE!");
			exit(1);
	}
	return move_lexer;
}

AST* token_handler(Token token, Lexer* lex)
{
	AST* ret = NULL;
	switch (token.type)
	{
	case TOKEN_NUMBER:
		ret = create_number_node(token_number(lex, token));
		break;
	case TOKEN_TRUE:
		ret = create_number_node(1);
		break;
	case TOKEN_FALSE:
		ret = create_number_node(0);
		break;

	case TOKEN_BINOP_ADD:
		ret = create_binary_op_node('+', NULL, NULL);
		break;
	case TOKEN_BINOP_SUB:
		ret = create_binary_op_node('-', NULL, NULL);
		break;
	case TOKEN_BINOP_MULT:
		ret = create_binary_op_node('*', NULL, NULL);
		break;
	case TOKEN_BINOP_DIV:
		ret = create_binary_op_node('/', NULL, NULL);
		break;

	// case TOKEN_UNOP_ADD:
	// 	ret = create_unary_op_node('+', NULL);
	// 	break;
	// case TOKEN_UNOP_SUB:
	// 	ret = create_unary_op_node('-', NULL);
	// 	break;
	case TOKEN_OPP:
		ret = create_unary_op_node('!', NULL);
		break;

	default:
		error_under_token(lex->source, lex->tok, "erreur token");
		exit(1);
	}

	return ret;
}

AST* reduce_rule(Rule rule, ASTStack* ast_stk)
{
	switch (rule) {
		case ADD_SUITE: {
			if (ast_stk->top >= 3) {
				AST* right = ast_stack_pop(ast_stk);
				AST* op = ast_stack_pop(ast_stk);
				AST* left = ast_stack_pop(ast_stk);
				
				if (op->type == BINARY_OP) {
					op->data.binary.left = left;
					op->data.binary.right = right;
					return op;
				}
			}
			return NULL;
		}
		
		case MULT_SUITE: {
			if (ast_stk->top >= 3) {
				AST* right = ast_stack_pop(ast_stk);
				AST* op = ast_stack_pop(ast_stk);
				AST* left = ast_stack_pop(ast_stk);
				
				if (op->type == BINARY_OP) {
					op->data.binary.left = left;
					op->data.binary.right = right;
					return op;
				}
			}
			return NULL;
		}
		
		case UNARY: {
			printf("unitary here\n");
			if (ast_stk->top >= 2) {
				AST* operand = ast_stack_pop(ast_stk);
				AST* top = ast_stack_peek(ast_stk);
				
				if (top && top->type == UNARY_OP) {
					AST* op = ast_stack_pop(ast_stk);
					op->data.unary.operand = operand;
					return op;
				}
				
				return operand;
			} else if (ast_stk->top == 1) {
				return ast_stack_pop(ast_stk);
			}
			return NULL;
		}
		
		case C_O_P: {
			if (ast_stk->top > 0) {
				AST* operand = ast_stack_pop(ast_stk);
				
				while (ast_stk->top > 0) {
					AST* top = ast_stack_peek(ast_stk);
					
					if (top && top->type == UNARY_OP && top->data.unary.operand == NULL) {
						AST* op = ast_stack_pop(ast_stk);
						op->data.unary.operand = operand;
						operand = op;
					} else {
						break;
					}
				}
				
				return operand;
			}
			return NULL;
		}
		case POSTFIX_EXPR:
		case PRIMARY:
			if (ast_stk->top > 0) {
				return ast_stack_pop(ast_stk);
			}
			return NULL;
		
		case POSTFIX_TAIL:
			return NULL;
		
		case ADD:
		case MULT:
			return NULL;
		
		default:
			parser_error("PARSER INTERNAL UNRECOGNIZED RULE IN REDUCE!");
			exit(1);
	}
}

AST* parse_main(Lexer* lex)
{
	Stack* stk = stack_init();
	ASTStack* astk = ast_stack_init();

	NUT start;
	start.type = RULE;
	start.rule = ADD;
	stack_push(stk, start);
	
	while (stk->top > 0) {
		
		NUT curr = stack_pop(stk);
		
		switch (curr.type) {
			case RULE:
				if (should_reduce(curr.rule, lex->tok)) {
					print_ast_parser(astk);
					AST* node = reduce_rule(curr.rule, astk);
					if (node) ast_stack_push(astk, node);
				} else {
					if (rule_handler(stk, curr.rule, lex))
					lexer_next(lex);
				}
				break;
			case TOKEN: {
				AST* node = token_handler(curr.token, lex);
				ast_stack_push(astk, node); 
				break;
			}
		}
	}
	print_ast_parser(astk);
	return ast_stack_pop(astk); 
}
