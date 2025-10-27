#ifndef BACKEND_PARSER_H
#define BACKEND_PARSER_H


#include "lexer/lexer.h"
#include "ast/ast.h"

#define PARSER_STACK_SIZE 256

typedef enum {
	ADD,
	ADD_SUITE,
	MULT,
	MULT_SUITE,
	UNARY,
	C_O_P,
	POSTFIX_EXPR,
	POSTFIX_TAIL,
	PRIMARY
} Rule;

// Node in the parser stack: either a rule (non-terminal) or token (terminal)
typedef struct {
	enum {RULE, TOKEN} type;
	union {
		Token token;
		Rule rule;
	};
} NUT;

typedef struct {
	NUT items[PARSER_STACK_SIZE];
	int top;
} Stack;

typedef struct {
	AST* items[PARSER_STACK_SIZE];
    size_t top;
} ASTStack;


Stack* stack_init();
void stack_push(Stack* stk, NUT value);
NUT stack_pop(Stack* stk);
void print_parser(Stack* stk);
ASTStack* ast_stack_init();
void ast_stack_push(ASTStack* stk, AST* value);
AST* ast_stack_pop(ASTStack* stk);
AST* ast_stack_peek(ASTStack* stk);
void print_ast_parser(ASTStack* stk);

#endif