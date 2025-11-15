#include "actions.h"

void action_suite_binop(void)
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

void action_suite_unop(void)
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
void action_stub(void) { return ; }
