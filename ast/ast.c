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
		
		case NUMBER:
		default:
			break;
	}
	free(node);
}

