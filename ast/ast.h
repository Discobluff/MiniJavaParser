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

	/*For Stmt rule*/
	DECSTMT, 	//type ident;
	DEF,		// type ident = expr
	OPT,		// if (stmt) {stmt} else {stmt}
	FOR, 		// for(expr;expr;expr)

	/*For Method*/
	METHOD,

	/*For Constructor*/
	CONS,

	/*For Decl*/
	DECL,

	/*For Class*/
	CLASS,

	/*For File*/
	FILE,

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

typedef struct{
	Type type;
	char* id; //not an AST: just name of the ident for the moment
}DeclstmtAST;

typedef struct{
	Type type;
	AST* expr;
	AST* id;
}DefAST;

typedef struct{
	AST* expr;
	int nb_stmt;
	AST** stmt;
}OptAST;

typedef struct{
	AST* expr;
}ReturnAST;

typedef struct{
	int nb_expr;
	AST** forexpr;
}ForAST;

typedef struct{
	Type type;
	AST* id;
	int nb_param;
	int nb_stmt;
	AST** methodparam;
	AST** methodstmt;
}MethodAST;

typedef struct{
	AST* id;
	int nb_param;
	int nb_stmt;
	AST** consparam;
	AST** consstmt;
}ConsAST;

typedef struct{
	Type type;
	char* id; 	//not an AST: just name of the ident for the moment
}DeclAST;

typedef struct{
	char* id;
	char* id1;
	int nb_decl;
	AST** classdecl;
}ClassAST;

typedef struct{
	int nb_class;
	AST** fileclass;
	AST* class_main;
}FileAST;


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
		DeclstmtAST declstmtast;
		DefAST defast;
		OptAST optast;
		ForAST forast;
		ReturnAST returnast;
		MethodAST methodast;
		ConsAST consast;
		DeclAST declast;
		ClassAST classast;
		FileAST fileast;
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