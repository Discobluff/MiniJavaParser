#ifndef AST_H
#define AST_H
#include <stdlib.h>

/**
 * Output of the parser input of the type checker.
 */

typedef enum {
	NUMBER,
	BINARY_OP,
	UNARY_OP,
	VARIABLE,
} ASTtype;

typedef struct AST AST;

typedef struct {
	char op;
	AST* left;
	AST* right;
} BinaryOp;

typedef struct {
	char op;
	AST* operand;
} UnaryOp;

struct AST {
	ASTtype type;
	union {
		size_t number;
		BinaryOp binary;
		UnaryOp unary;
		char* variable;
	} data;
};


AST* create_number_node(size_t value);
AST* create_binary_op_node(char op, AST* left, AST* right);
AST* create_unary_op_node(char op, AST* operand);
AST* create_variable_node(char* name, int size);

size_t evaluate(AST *node);
void print_ast(AST *node, int depth);

void free_ast(AST *node);

#endif