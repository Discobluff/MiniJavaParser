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

char* rule_to_string(Rule rule)
{
	switch (rule)
	{
	case FIL: 
		return "FIL";
	case CLASS_LIST: 
		return "CLASS_LIST";
	case CLASSE: 
		return "CLASSE";
	case PUBLIC_OPT: 
		return "PUBLIC_OPT";
	case STATIC_OPT: 
		return "STATIC_OPT";
	case EXTENDS_OPT: 
		return "EXTENDS_OPT";
	case DECL_LIST: 
		return "DECL_LIST";
	case DECL_TYPE: 
		return "DECL_TYPE";
	case DECL_TYPE_IDENT: 
		return "DECL_TYPE_IDENT";
	case DECLA: 
		return "DECLA";
	case DECL_PUBLIC: 
		return "DECL_PUBLIC";
	case CONST_TAIL: 
		return "CONST_TAIL";
	case CONST: 
		return "CONST";
	case PARAMS_OPT: 
		return "PARAMS_OPT";
	case PARAMS: 
		return "PARAMS";
	case PARAMS2: 
		return "PARAMS2";
	case TYPE: 
		return "TYPE";
	case EXPR_OPT: 
		return "EXPR_OPT";
	case EXPR: 
		return "EXPR";
	case ASSIGN:
		return "ASSIGN";
	case ASSIGN_ID:
		return "ASSIGN_ID";
	case LOR_EXPR:
		return "LOR_EXPR";
	case LOR_EXPR_SUITE:
		return "LOR_EXPR_SUITE";
	case LAND_EXPR:
		return "LAND_EXPR";
	case LAND_EXPR_SUITE:
		return "LAND_EXPR_SUITE";
	case EQ_EXPR:
		return "EQ_EXPR";
	case EQ_EXPR_SUITE:
		return "EQ_EXPR_SUITE";
	case RELATIONAL_EXPR:
		return "RELATIONAL_EXPR";
	case RELATIONAL_EXPR_SUITE:
		return "RELATIONAL_EXPR_SUITE";
	case ADD:
		return "ADD";
	case ADD_SUITE:
		return "ADD_SUITE";
	case MULT:
		return "MULT";
	case MULT_SUITE:
		return "MULT_SUITE";
	case UNARY:
		return "UNARY";
	case C_O_P:
		return "C_O_P";
	case C_O_P_PAR:
		return "C_O_P_PAR";
	case POSTFIX_EXPR:
		return "POSTFIX_EXPR";
	case POSTFIX_TAIL:
		return "POSTFIX_TAIL";
	case PRIMARY:
		return "PRIMARY";
	case PRIMARY_PAR:
		return "PRIMARY_PAR";
	case LEXPR_OPT:
		return "LEXPR_OPT";
	case LEXPR:
		return "LEXPR";
	case LEXPR_EXPR:
		return "LEXPR_EXPR";
	case STMT_LIST:
		return "STMT_LIST";
	case STMT:
		return "STMT";
	case STMT_TYPE_IDENT:
		return "STMT_TYPE_IDENT";
	case STMT_IF:
		return "STMT_IF";
	case BLOCK:
		return "BLOCK";
	case METHODE:
		return "METHODE";
	case CONST_OR_METH:
		return "CONST_OR_METH";
	case CRO_OPT:
		return "CRO_OPT";
	case CONST_OR_METH_OR_ATTR:
		return "CONST_OR_METH_OR_ATTR";
	case FUNCTION_TAIL:
		return "FUNCTION_TAIL";
	case METH_OR_ATTR:
		return "METH_OR_ATTR";
	case CONST_OR_METH_SOL:
		return "CONST_OR_METH_SOL";
	case PARAMS_TAIL:
		return "PARAMS_TAIL";
	case ASSIGNMENT:
		return "ASSIGNMENT";
	case ASSIGNEMENT_REC:
		return "ASSIGNEMENT_REC";
	case STMT_IDENT:
		return "STMT_IDENT";
	case ASSIGNEMENT_REC_IDENT:
		return "ASSIGNEMENT_REC_IDENT";
	default:
		return "UNKNOWN_RULE???? :((((";
	}
}

void print_parser(Stack* stk)
{
	printf("Parser Stack (top -> bottom):\n");
	for (int i = stk->top - 1; i >= 0; i--) {
		NUT nut = stk->items[i];
		if (nut.type == RULE) {
			printf("  [RULE] %s\n", rule_to_string(nut.rule));
		} else {
			printf("  [TOKEN] %s\n", token_type_name(nut.token));
		}
	}
	printf("End Parser Stack\n");
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
	printf("Parser AST Stack (top -> bottom):\n");
	for (int i = (int)stk->top - 1; i > 0; i--) {
		print_ast(stk->items[i], 0);
		printf("\n");
	}
	print_ast(stk->items[0], 0);
	printf("End AST Parser Stack\n\n");
}
