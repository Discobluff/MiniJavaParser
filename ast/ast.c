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

AST* create_binary_op_node(TokenType op, AST* left, AST* right)
{
	AST *node = malloc(sizeof(AST));
	node->type = BINARY_OP;
	node->data.binary.op = op;
	node->data.binary.left = left;
	node->data.binary.right = right;
	return node;
}

AST* create_unary_op_node(TokenType op, AST* operand)
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

AST* create_declstmt_node (Type type, char* id){
	AST *node = malloc(sizeof(AST));
	node-> type = DECLSTMT;
	node->data.declstmtast.type=type;
	node->data.declstmtast.id=id;
	return node;
}

AST* create_def_node(Type type, AST* id, AST* expr){
	AST *node = malloc(sizeof(AST));
	node->type = DEF;
	node->data.defast.type= type;
	node->data.defast.id = id;
	node->data.defast.expr = expr;
	return node;
}

AST* create_opt_node(AST* expr, int nb_stmt){
	AST *node= malloc(sizeof(AST));
	node->type=OPT;
	node->data.optast.expr =expr;
	node->data.optast.nb_stmt=nb_stmt;
	node->data.optast.stmt= malloc(nb_stmt*sizeof(AST)); 
	return node;
}

AST* create_return_node(AST *expr){
	AST *node=malloc(sizeof(AST));
	node->type=RETURN;
	node->data.returnast.expr =expr;
	return node;
}

AST* create_for_node(int nb_expr){
	AST *node=malloc(sizeof(AST));
	node->type=FOR;
	node->data.forast.nb_expr=nb_expr;
	node->data.forast.forexpr=malloc(nb_expr* sizeof(AST));
	return node;
}

AST* create_method_node(Type type, AST* id, int nb_param, int nb_stmt){
	AST *node=malloc(sizeof(AST));
	node->type=METHOD;
	node->data.methodast.type=type;
	node->data.methodast.id= id;
	node->data.methodast.nb_param=nb_param;
	node->data.methodast.methodparam=malloc(nb_param *sizeof(AST));
	node->data.methodast.nb_stmt=nb_stmt;
	node->data.methodast.methodstmt=malloc(nb_stmt *sizeof(AST));
	return node;
}

AST* create_cons_node(AST *id, int nb_param, int nb_stmt){
	AST *node=malloc(sizeof(AST));
	node->type=CONS;
	node->data.consast.id= id;
	node->data.consast.nb_param=nb_param;
	node->data.consast.consparam=malloc(nb_param *sizeof(AST));
	node->data.consast.nb_stmt=nb_stmt;
	node->data.consast.consstmt=malloc(nb_stmt *sizeof(AST));
	return node;
}

AST* create_decl_node(Type type, char* id){
	AST *node=malloc(sizeof(AST));
	node->type=DECL;
	node->data.declast.type=type;
	node->data.declast.id=id;
	return node;
}

AST* create_class_node(char *id, char *id1, int nb_decl){
	AST *node=malloc(sizeof(AST));
	node->type=CLASS;
	node->data.classast.id=id;
	node->data.classast.id1=id1;
	node->data.classast.nb_decl=nb_decl;
	node->data.classast.classdecl=malloc(nb_decl *sizeof(AST));
	return node;
}

AST* create_file_node( int nb_class, AST* class_main){
	AST *node=malloc(sizeof(AST));
	node->type=FILES;
	node->data.fileast.nb_class=nb_class;
	node->data.fileast.fileclass=malloc(nb_class *sizeof(AST));
	node->data.fileast.class_main=class_main;
	return node;
}

int max(int a, int b){
	return a > b ? a : b;
}

int maxList(AST **astList, int len){
	int res = -1;
	for (int i=0; i<len; i++){
		res = max(res, depth(astList[i]));
	}
	return res;
}

int depth(AST *ast){
	if (ast == NULL)
		return 0;

	switch (ast->type){
		case NUMBER:
		case VARIABLE:
		case PARAM:
		case DECLSTMT:
		case DECL:
			return 0;
		case UNARY_OP:
			return 1 + depth(ast->data.unary.operand);
		case BINARY_OP:
			return 1 + max(depth(ast->data.binary.left),depth(ast->data.binary.right));
		case DOT:
			return 1 + max(depth(ast->data.dotast.id),depth(ast->data.dotast.expr));
		case AFFECT:
			return 1 + max(depth(ast->data.affectast.id),depth(ast->data.affectast.value));
		case FUNC_CALL:
			return 1 + max(depth(ast->data.funcallast.id),maxList(ast->data.funcallast.func_arguments, ast->data.funcallast.nb_arguments));
		case CAST:
			return 1 + depth(ast->data.castast.expr);
		case INSTANCEOF:
			return 1 + depth(ast->data.instofast.expr);
		case DEF:
			return 1 + max(depth(ast->data.defast.expr),depth(ast->data.defast.id));
		case OPT:
			return 1 + max(depth(ast->data.optast.expr),maxList(ast->data.optast.stmt, ast->data.optast.nb_stmt));
		case RETURN:
			return 1 + depth(ast->data.returnast.expr);
		case FOR:
			return 1 + maxList(ast->data.forast.forexpr, ast->data.forast.nb_expr);
		case METHOD:
			return 1 + max(max(depth(ast->data.methodast.id),maxList(ast->data.methodast.methodparam, ast->data.methodast.nb_param)),maxList(ast->data.methodast.methodstmt, ast->data.methodast.nb_stmt));
		case CONS:
			return 1 + max(max(depth(ast->data.consast.id),maxList(ast->data.consast.consparam, ast->data.consast.nb_param)),maxList(ast->data.consast.consstmt, ast->data.consast.nb_stmt));
		case CLASS:
		return 1 + maxList(ast->data.classast.classdecl, ast->data.classast.nb_decl);
		case FILES:
			return 1 + max(depth(ast->data.fileast.class_main),maxList(ast->data.fileast.fileclass, ast->data.fileast.nb_class));
		default:
			return -1;
	}
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
				case TOKEN_BINOP_ADD: return left + right;
				case TOKEN_BINOP_SUB: return left - right;
				case TOKEN_BINOP_MULT: return left * right;
				case TOKEN_BINOP_DIV: return left / right;
				default: return 0;
			}
		}
		
		case UNARY_OP: {
			size_t operand = evaluate(node->data.unary.operand);
			switch (node->data.unary.op) {
				case TOKEN_BINOP_SUB: return -operand;
				case TOKEN_OPP: return !operand;
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
			printf("BinaryOp: %s\n", token_type_name(node->data.binary.op));
			print_ast(node->data.binary.left, depth + 1);
			print_ast(node->data.binary.right, depth + 1);
			break;
		
		case UNARY_OP:
			printf("UnaryOp: %s\n", token_type_name(node->data.unary.op));
			print_ast(node->data.unary.operand, depth + 1);
			break;
		
		case VARIABLE:
			printf("Variable: %s\n", node->data.variable);
			break;
		
		default:
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
			break;

		case INSTANCEOF :
			free_ast(node->data.instofast.expr);	
			break;
	
		case PARAM :
			free(node->data.paramast.id);
			break;

		case NUMBER:
		default:
			break;

		case DECLSTMT:
			free(node->data.declstmtast.id);
			break;

		case DEF:
			free_ast(node->data.defast.id);
			free_ast(node->data.defast.expr);
			break;
		
		case OPT:
			free_ast(node->data.optast.expr);
			for (int i=0; i < node->data.optast.nb_stmt; i++) 
				free_ast(node->data.optast.stmt[i]);
			break;

		case RETURN:
			free_ast(node->data.returnast.expr);
			break;
		
		case FOR:
			for (int i=0; i < node->data.forast.nb_expr; i++) 
				free_ast(node->data.forast.forexpr[i]);
			break;

		case METHOD:
			free_ast(node->data.methodast.id);
			for (int i=0; i < node->data.methodast.nb_param; i++) 
				free_ast(node->data.methodast.methodparam[i]);
			for (int i=0; i < node -> data.methodast.nb_stmt; i++)
				free_ast(node->data.methodast.methodstmt[i]);
			break;
		
		case CONS:
			free_ast(node->data.consast.id);
			for (int i=0; i < node->data.consast.nb_param; i++) 
				free_ast(node->data.consast.consparam[i]);
			for (int i=0; i < node -> data.consast.nb_stmt; i++)
				free_ast(node->data.consast.consstmt[i]);
			break;
		
		case DECL:
			free(node->data.declast.id);
			break;

		case CLASS:
			free(node->data.classast.id);
			// free_ast(node->data.classast.id1);
			for (int i=0; i < node->data.classast.nb_decl; i++) 
				free_ast(node->data.classast.classdecl[i]);
			break;

		case FILES:
			free_ast(node->data.fileast.class_main);
			for (int i=0; i < node->data.fileast.nb_class; i++) 
				free_ast(node->data.fileast.fileclass[i]);
			break;

	}
	free(node);
}

