#include "actions.h"

void action_binop_create(Lexer* lex)
{
	AST* value = ast_stack_pop(astk);

	AST* node = create_binary_op_node(lexer_giveToken(lex).type, value, NULL);

	ast_stack_push(astk, node);
	return ;
}

void action_unop_create(Lexer* lex)
{
	AST* node = create_unary_op_node(lexer_giveToken(lex).type, NULL);

	ast_stack_push(astk, node);
	return ;
}

void action_binop_end(__attribute__((unused))Lexer* _lex)
{
	if (astk->top < 2)
		return ; // cannot reduce 

	    AST* value = ast_stack_pop(astk);
    
    // Keep combining with binary operators while they exist
    while (astk->top >= 1)
    {
        AST* operator_node = ast_stack_peek(astk);
        
        // Check if it's a binary operator waiting for a right operand
        if (operator_node && operator_node->type == BINARY_OP && operator_node->data.binary.right == NULL)
        {
            ast_stack_pop(astk); // remove the operator
            operator_node->data.binary.right = value;
            value = operator_node; // this becomes the new value
        }
        else
        {
            break; // no more binary operators waiting
        }
    }
	ast_stack_push(astk, value);
	return ;
}

void action_unop_end(__attribute__((unused)) Lexer* _lex)
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

void action_create_number_leaf(Lexer* lex)
{
	Token tok = lexer_giveToken(lex);

	AST* number_node = create_number_node(token_number(lex, tok));
	ast_stack_push(astk, number_node);
	return ;
}

void action_create_boolean_leaf(Lexer* lex)
{
	Token tok = lexer_giveToken(lex);

	size_t value = (tok.type == TOKEN_TRUE) ? 1 : 0;
	AST* boolean_node = create_number_node(value);
	ast_stack_push(astk, boolean_node);
	return ;
}

// what the sigma? used for not having NULL as the action function (so we know when we have an error in the main loop of the parser)
void action_stub(__attribute__((unused)) Lexer* _lex) { return ; }