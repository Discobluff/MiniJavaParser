#include "ast.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

AST* create_number_node(size_t value)
{
	AST *node = malloc(sizeof(AST));
	node->type = NUMBER;
	node->data.number = value;
	return node;
}

AST* create_binary_op_node(char op, AST* left, AST* right)
{
	AST *node = malloc(sizeof(AST));
	node->type = BINARY_OP;
	node->data.binary.op = op;
	node->data.binary.left = left;
	node->data.binary.right = right;
	return node;
}

AST* create_unary_op_node(char op, AST* operand)
{
	AST *node = malloc(sizeof(AST));
	node->type = UNARY_OP;
	node->data.unary.op = op;
	node->data.unary.operand = operand;
	return node;
}

// The idea is to pass the name with token_text TODO: TEST THIS!!!!!
AST* create_variable_node(char* name, int size)
{
	AST *node = malloc(sizeof(AST));
	node->type = VARIABLE;
	node->data.variable = malloc(size);
	strncpy(name, node->data.variable, size);
	return node;
}


AST* create_dot_node (AST* expr, AST* id) {
	AST *node = malloc(sizeof(AST));
	node->type = DOT;
	node->data.dotast.expr = expr;
	node->data.dotast.id = id;
	return node;
}


AST* create_affect_node (AST* id, AST* value) {
	AST *node = malloc(sizeof(AST));
	node->type = AFFECT;
	node->data.affectast.id = id;
	node->data.affectast.value = value;
	return node;
}


AST* create_funcall_node (AST* id, int nb_arguments) {
	AST* node = malloc(sizeof(AST));
	node->type = FUNC_CALL;
	node->data.funcallast.id = id;
	node->data.funcallast.nb_arguments = nb_arguments;
	node->data.funcallast.func_arguments = malloc(nb_arguments * sizeof(AST*));
	return node;
}

AST* create_cast_node (Type type, AST* expr) {
	AST *node = malloc(sizeof(AST));
	node->type = CAST;
	node->data.castast.type = type;
	node->data.castast.expr = expr;
	return node;
}

AST* create_instof_node (Type type, AST* expr) {
	AST *node = malloc(sizeof(AST));
	node->type = INSTANCEOF;
	node->data.instofast.type = type;
	node->data.instofast.expr = expr;
	return node;
}

AST* create_param_node (Type type, char* id) {
	AST *node = malloc(sizeof(AST));
	node->type = PARAM;
	node->data.paramast.type = type;
	node->data.paramast.id = id;
	return node;
}
// Evaluate
size_t evaluate(AST *node) 
{
	switch (node->type) {
		case NUMBER:
			return node->data.number;
		
		case BINARY_OP: {
			size_t left = evaluate(node->data.binary.left);
			size_t right = evaluate(node->data.binary.right);
			switch (node->data.binary.op) {
				case '+': return left + right;
				case '-': return left - right;
				case '*': return left * right;
				case '/': return left / right;
				default: return 0;
			}
		}
		
		case UNARY_OP: {
			size_t operand = evaluate(node->data.unary.operand);
			switch (node->data.unary.op) {
				case '-': return -operand;
				case '!': return !operand;
				default: return operand;
			}
		}
		
		default:
			printf("Other lookup not pertinent\n");
			return 0;
	}
	return 0;
}

// Print the AST (parcours en profondeur)
void print_ast(AST *node, int depth) 
{
	if (!node) return;

	for (int i = 0; i < depth; i++) printf("  ");

	switch (node->type) {
		case NUMBER:
			printf("Number: %ld\n", node->data.number);
			break;
		
		case BINARY_OP:
			printf("BinaryOp: %c\n", node->data.binary.op);
			print_ast(node->data.binary.left, depth + 1);
			print_ast(node->data.binary.right, depth + 1);
			break;
		
		case UNARY_OP:
			printf("UnaryOp: %c\n", node->data.unary.op);
			print_ast(node->data.unary.operand, depth + 1);
			break;
		
		case VARIABLE:
			printf("Variable: %s\n", node->data.variable);
			break;
	}
}

// Free the AST
void free_ast(AST *node)
{
	if (!node) return;
	
	switch (node->type) {
		case BINARY_OP:
			free_ast(node->data.binary.left);
			free_ast(node->data.binary.right);
			break;
		
		case UNARY_OP:
			free_ast(node->data.unary.operand);
			break;
		
		case VARIABLE:
			free(node->data.variable);
			break;
		
		case DOT:
			free_ast(node->data.dotast.expr);
			free_ast(node->data.dotast.id);
			break;

		case AFFECT:	
			free_ast(node->data.affectast.id);
			free_ast(node->data.affectast.value);	
			break;

		case FUNC_CALL :
			free_ast(node->data.funcallast.id);
			for (int i=0; i < node->data.funcallast.nb_arguments ; i++) 
				free_ast(node->data.funcallast.func_arguments[i]);
			break;

		case CAST :
			free_ast(node->data.castast.expr);

		case INSTANCEOF :
			free_ast(node->data.instofast.expr);	
	
		case PARAM :
			free_ast(node->data.paramast.id);

		case NUMBER:
		default:
			break;
	}
	free(node);
}

