#include "backend_parser.h"
#include "error/print_error.h"
#include <stdio.h>

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
    ASTStack* stk = malloc(sizeof(ASTStack));
    stk->top = 0;
    return stk;
}

void ast_stack_push(ASTStack* stk, AST* node)
{
	if (node == NULL) return;

	if (stk->top < PARSER_STACK_SIZE) {
		stk->items[stk->top] = node;
		stk->top++;
	} else {
		parser_error("PARSER INTERNAL AST STACK OVERFLOW!");
		exit(1);
	}
}

AST* ast_stack_pop(ASTStack* stk)
{
	if (stk->top > 0)
		return stk->items[--stk->top];
	return NULL;
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
