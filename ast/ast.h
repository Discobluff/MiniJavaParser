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

	/* For Expr rule */

	DOT,		// expr.id
	AFFECT,		// expr = expr
	FUNC_CALL,	// id (lexpr?)
	NEW,  		// new id (lexpr?)
	CAST, 		// (type) expr
	INSTANCEOF,	// expr instanceof type

	/* For Params*/
	PARAM,

} ASTtype;


typedef enum {
	INT,
	BOOL,
	ID,
} Type;

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

typedef struct {
	AST* expr;
	AST* id;
} DotAST;

typedef struct {
	AST* id;
	AST* value;
} AffectAST;

typedef struct {
	int nb_arguments;
	AST* id;
	AST** func_arguments;
} FuncCallAST;

typedef struct {
	Type type;
	AST* expr;
} CastAST;

typedef struct {
	Type type;
	AST* expr;
} InstOfAST;

typedef struct {
	Type type;
	char* id; //not an AST : just name of the parameter for the moment
} ParamAST;


struct AST {
	ASTtype type;
	union {
		size_t number;
		char* variable;
		BinaryOp binary;
		UnaryOp unary;
		DotAST dotast;
		AffectAST affectast;
		FuncCallAST funcallast;
		CastAST castast;
		InstOfAST instofast;
		ParamAST paramast;
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