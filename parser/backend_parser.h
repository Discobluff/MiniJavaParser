#ifndef BACKEND_PARSER_H
#define BACKEND_PARSER_H


#include "lexer/lexer.h"
#include "ast/ast.h"

#define PARSER_STACK_SIZE 256

typedef enum {	
	FIL,
	CLASS_LIST,
	CLASSE,
	PUBLIC_OPT,
	STATIC_OPT,
	EXTENDS_OPT,
	DECL_LIST,
	DECL_TYPE,
	DECL_TYPE_IDENT,
	DECLA,
	DECL_PUBLIC,
	CONST_TAIL,
	CONST,
	PARAMS_OPT,
	PARAMS,
	PARAMS2,
	TYPE,
	EXPR_OPT,
	EXPR,
	ASSIGN,
	ASSIGN_ID,
	LOR_EXPR,
	LOR_EXPR_SUITE,
	LAND_EXPR,
	LAND_EXPR_SUITE,
	EQ_EXPR,
	EQ_EXPR_SUITE,
	RELATIONAL_EXPR,
	RELATIONAL_EXPR_SUITE,
	ADD,
	ADD_SUITE,
	MULT,
	MULT_SUITE,
	UNARY,
	C_O_P,
	C_O_P_PAR,
	POSTFIX_EXPR,
	POSTFIX_TAIL,
	PRIMARY,
	PRIMARY_PAR,
	LEXPR_OPT,
	LEXPR,
	LEXPR_EXPR,
	STMT_LIST,
	STMT,
	STMT_TYPE_IDENT,
	STMT_IF,
	BLOCK,

	METHODE,
	CONST_OR_METH,

	// TYPE_DEF,
	// STMT_IDENT,

	// leave at the end
	RULE_SIZE
} Rule;

// Node in the parser stack: either a rule (non-terminal) or token (terminal)
typedef struct {
	enum {RULE, TOKEN} type;
	union {
		TokenType token;
		Rule rule;
	};
} NUT;

typedef struct {
	NUT items[PARSER_STACK_SIZE];
	size_t top;
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
