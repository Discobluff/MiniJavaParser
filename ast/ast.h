#ifndef AST_H
#define AST_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Type Type;
typedef struct Expression Expression;
typedef struct Statement Statement;

typedef enum {
	TYPE_INT,
	TYPE_BOOLEAN,
	TYPE_STRING,
	TYPE_VOID,
	TYPE_CLASS,
	TYPE_NULL,
	TYPE_ERROR,
} TypeKind;

/**
 * Generic type int, bool, string, void and class values
 */
struct Type {
	TypeKind kind;
	char* class_name;		// For TYPE_CLASS
};

typedef enum {
	EXPR_LITERAL,
	EXPR_VARIABLE,
	EXPR_BINARY_OP,
	EXPR_UNARY_OP,
	EXPR_CALL,
	EXPR_INSTOF,
	EXPR_NEW,
	EXPR_FIELD_ACCESS,
	EXPR_CAST
} ExprKind;

typedef enum {
	OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
	OP_EQ, OP_NEQ, OP_LT, OP_LTE, OP_GT, OP_GTE,
	OP_AND, OP_OR,
	OP_ASSIGN
} BinaryOp;

typedef enum {
	UNARY_NOT, UNARY_NEG,
} UnaryOp;

typedef enum {
	LIT_INT, LIT_BOOLEAN, LIT_STRING, LIT_NULL
} LiteralKind;

/**
 * Type and value of a literal initialised
 */
typedef struct {
	LiteralKind kind;
	union {
		int int_val;
		int bool_val;
		char* string_val;
	};
} Literal;

/**
 * callee(args)
 */
typedef struct {
	Expression* callee;

	Expression** args;
	int arg_count;
} CallExpr;

/**
 * new type(args)
 * 
 * Here type should be of TYPE_CLASS
 */
typedef struct {
	Type* type;
	Expression** args;
	int arg_count;
} NewExpr;

/**
 * Expresssions are of kind:
 * 	-
 */
struct Expression {
	ExprKind kind;
	Type* type;  // Type of the expression (filled during type checking)
	union {
		Literal literal;
		char* var_name;
		struct {
			BinaryOp op;
			Expression* left;
			Expression* right;
		} binary;
		struct {
			UnaryOp op;
			Expression* operand;
		} unary;
		CallExpr call;
		struct {
			Expression* val;
			Type* comp;
		} instof;
		NewExpr new_expr;
		struct {
			Expression* object;
			char* field_name;
		} field_access;
		struct {
			Type* target_type;
			Expression* expr;
		} cast;
	};
};

typedef enum {
	STMT_EXPR,
	STMT_RETURN,
	STMT_IF,
	STMT_FOR,
	STMT_VAR_DECL,
	STMT_BREAK,
	STMT_CONTINUE,
	STMT_BLOCK,
} StmtKind;

/**
 * { block }
 * represents the scopes of the variables.
 */
typedef struct {
	Statement** stmts;
	int count;
} Block;

typedef struct {
	Expression* condition;
	Statement* then_branch;
	Statement* else_branch;	// Can be NULL
} IfStmt;

typedef struct {
	Statement* init;  		// Can be NULL
	Expression* condition;	// Can be NULL
	Expression* increment;	// Can be NULL
	Statement* body;
} ForStmt;

/**
 * type name = initialiser;
 */
typedef struct {
	Type* type;
	char* name;
	Expression* initializer;  // Can be NULL
} VarDecl;

struct Statement {
	StmtKind kind;
	union {
		Expression* expr;
		Expression* return_val;  // Can be NULL for void returns
		IfStmt if_stmt;
		ForStmt for_stmt;
		Block block;
		VarDecl var_decl;
	};
};

/**
 * general modifiers biteflags
 */
typedef enum {
	MOD_PUBLIC = 1 << 0,
	MOD_PRIVATE = 1 << 1,
	MOD_STATIC = 1 << 2,
} Modifier;

/**
 * modifier type name = initialiser;
 */
typedef struct {
	int modifiers;
	Type* type;
	char* name;
	Expression* initializer;  // Can be NULL
} Field;

/**
 * type name
 */
typedef struct {
	Type* type;
	char* name;
} Parameter;

/**
 * modifiers return_type name(params) { body }
 */
typedef struct {
	int modifiers;

	Type* return_type;
	char* name;

	Parameter** params;
	int param_count;

	Statement* body;  // Block statement
} Method;

/**
 * modifiers name(params) { body }
 */
typedef struct {
	int modifiers;
	
	Type* type;

	Parameter** params;
	int param_count;

	Statement* body;
} Constructor;

/**
 * 	modifiers name super_class {
 * 		Fields
 * 		constructors
 * 		methods
 * 	}
 *  (not in order)
 */
typedef struct {
	int modifiers;
	char* name;
	char* super_class;  // Can be NULL

	Field** fields;
	int field_count;

	Constructor** constructors;
	int constructor_count;

	Method** methods;
	int method_count;
} ClassDecl;

/**
 * A file is a list of classes
 */
typedef struct {
	int class_count;
	ClassDecl** classes;
} File;

typedef enum{
	AST_TYPE,
	AST_NAME,
	AST_EXPRESSION,
	AST_STATEMENT,
	AST_PARAMETER,
	AST_METHOD,
	AST_CONSTRUCTOR,
	AST_CLASS_DECL,
	AST_FIELD,
	AST_FILE,
} TYPE_AST;

typedef struct{
	TYPE_AST type;
	union{
		Type *type;
		Expression *expression;
		Statement *statement;
		ClassDecl *class_decl;
		File *file;
		Parameter *parameter;
		Method *method;
		Constructor *constructor;
		Field *field;
		char *name;
	} data;
} AST;

Type* create_type(TypeKind kind);
Type* create_class_type(char* class_name);
Type* copy_type(Type* type);

Expression* create_literal_int(int val);
Expression* create_literal_boolean(int val);
Expression* create_literal_string(char* val);
Expression* create_literal_null();
Expression* create_variable(char* name);
Expression* create_binary_op(BinaryOp op, Expression* left, Expression* right);
Expression* create_unary_op(UnaryOp op, Expression* operand);
Expression* create_call(Expression* callee, Expression** args, int arg_count);
Expression* create_instaceof(Expression* val, Type* comp);
Expression* create_new(Type* type, Expression** args, int arg_count);
Expression* create_dot(Expression* object, char* field_name);
Expression* create_cast(Type* target_type, Expression* expr);


Statement* create_expr_stmt(Expression* expr);
Statement* create_return_stmt(Expression* val);
Statement* create_if_stmt(Expression* condition, Statement* then_branch, Statement* else_branch);
Statement* create_for_stmt(Statement* init, Expression* condition, Expression* increment, Statement* body);
Statement* create_block(Statement** stmts, int count);
Statement* create_var_decl(Type* type, char* name, Expression* init);
Statement* create_break_stmt();
Statement* create_continue_stmt();


Parameter* create_parameter(Type* type, char* name);

Field* create_field(int modifiers, Type* type, char* name, Expression* initializer);

Method* create_method(int modifiers, Type* return_type, char* name, Parameter** params, int param_count, Statement* body);

Constructor* create_constructor(int modifiers, Type* type, Parameter** params, int param_count, Statement* body);

ClassDecl* create_class(char* name);
void class_set_superclass(ClassDecl* cls, char* super_class);
void class_set_modifiers(ClassDecl* class, int modifiers);
void class_add_field(ClassDecl* cls, Field* field);
void class_add_constructor(ClassDecl* cls, Constructor* constructor);

void class_add_method(ClassDecl* cls, Method* method);

File* create_file();
void file_add_class(File* file, ClassDecl* cls);

void freeFile(File* file);
void freeClass(ClassDecl* class);
void freeMethod(Method* meth);
void freeConstructor(Constructor* constr);
void freeField(Field* field);
void freeParameter(Parameter* param);
void freeStatement(Statement* state);
void freeVarDecl(VarDecl var);
void freeIf(IfStmt ifstmt);
void freeFor(ForStmt forstmt);
void freeBlock(Block blk);
void freeCallExpr(CallExpr cexpr);
void freeNew(NewExpr n);
void freeExpression(Expression* expr);
void freeLiteral(Literal l);
void freeType(Type* type);


#endif
