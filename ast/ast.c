#include <ast/ast.h>

// ======================================= Type Constructors =======================================

Type* create_type(TypeKind kind)
{
	Type* t = malloc(sizeof(Type));
	t->kind = kind;
	t->class_name = NULL;
	return t;
}

Type* create_class_type(char* class_name)
{
	Type* t = malloc(sizeof(Type));
	t->kind = TYPE_CLASS;
	t->class_name = strdup(class_name);
	return t;
}

Type* create_string_type() {
	Type* t = malloc(sizeof(Type));
	t->kind = TYPE_STRING;
	t->class_name = NULL;
	return t;
}

// ======================================= Expression Constructors =======================================

Expression* create_literal_int(int val)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_LITERAL;
	e->type = create_type(TYPE_INT);
	e->literal.kind = LIT_INT;
	e->literal.int_val = val;
	return e;
}

Expression* create_literal_boolean(int val)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_LITERAL;
	e->type = create_type(TYPE_BOOLEAN);
	e->literal.kind = LIT_BOOLEAN;
	e->literal.bool_val = val;
	return e;
}

Expression* create_literal_string(char* val)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_LITERAL;
	e->type = create_type(TYPE_STRING);
	e->literal.kind = LIT_STRING;
	e->literal.string_val = strdup(val);

	return e;
}

Expression* create_literal_null()
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_LITERAL;
	e->type = NULL;  // null has no specific type
	e->literal.kind = LIT_NULL;
	return e;
}

Expression* create_variable(char* name)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_VARIABLE;
	e->type = NULL;  // To be filled during type checking
	e->var_name = strdup(name);
	return e;
}

Expression* create_binary_op(BinaryOp op, Expression* left, Expression* right)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_BINARY_OP;
	e->type = NULL;  // To be filled during type checking
	e->binary.op = op;
	e->binary.left = left;
	e->binary.right = right;
	return e;
}

Expression* create_unary_op(UnaryOp op, Expression* operand)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_UNARY_OP;
	e->type = NULL;  // To be filled during type checking
	e->unary.op = op;
	e->unary.operand = operand;
	return e;
}

Expression* create_call(Expression* callee, Expression** args, int arg_count)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_CALL;
	e->type = NULL;  // To be filled during type checking
	e->call.callee = callee;
	e->call.args = args;
	e->call.arg_count = arg_count;
	return e;
}

Expression* create_instaceof(Expression* val, Type* comp){
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_INSTOF;
	e->type = create_type(TYPE_BOOLEAN); // Type of a instanceof B is always bool
	e->instof.comp = comp;
	e->instof.val = val;
	return e;
}

Expression* create_new(Type* type, Expression** args, int arg_count)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_NEW;
	e->type = type;
	e->new_expr.type = type;
	e->new_expr.args = args;
	e->new_expr.arg_count = arg_count;
	return e;
}

Expression* create_dot(Expression* object, char* field_name)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_FIELD_ACCESS;
	e->type = NULL;  // To be filled during type checking
	e->field_access.object = object;
	e->field_access.field_name = strdup(field_name);
	return e;
}

Expression* create_cast(Type* target_type, Expression* expr)
{
	Expression* e = malloc(sizeof(Expression));
	e->kind = EXPR_CAST;
	e->type = target_type;
	e->cast.target_type = target_type;
	e->cast.expr = expr;
	return e;
}

// ======================================= Statement Constructors =======================================

Statement* create_expr_stmt(Expression* expr)
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_EXPR;
	s->expr = expr;
	return s;
}

Statement* create_return_stmt(Expression* val)
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_RETURN;
	s->return_val = val;
	return s;
}

Statement* create_if_stmt(Expression* condition, Statement* then_branch, Statement* else_branch)
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_IF;
	s->if_stmt.condition = condition;
	s->if_stmt.then_branch = then_branch;
	s->if_stmt.else_branch = else_branch;
	return s;
}

Statement* create_for_stmt(Statement* init, Expression* condition, Expression* increment, Statement* body)
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_FOR;
	s->for_stmt.init = init;
	s->for_stmt.condition = condition;
	s->for_stmt.increment = increment;
	s->for_stmt.body = body;
	return s;
}

Statement* create_block(Statement** stmts, int count)
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_BLOCK;
	s->block.stmts = stmts;
	s->block.count = count;
	return s;
}

Statement* create_var_decl(Type* type, char* name, Expression* init)
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_VAR_DECL;
	s->var_decl.type = type;
	s->var_decl.name = strdup(name);
	s->var_decl.initializer = init;
	return s;
}

Statement* create_break_stmt()
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_BREAK;
	return s;
}

Statement* create_continue_stmt()
{
	Statement* s = malloc(sizeof(Statement));
	s->kind = STMT_CONTINUE;
	return s;
}

// ======================================= Parameter Constructor =======================================

Parameter* create_parameter(Type* type, char* name)
{
	Parameter* p = malloc(sizeof(Parameter));
	p->type = type;
	p->name = strdup(name);
	return p;
}

// ======================================= Field Constructor =======================================

Field* create_field(int modifiers, Type* type, char* name, Expression* initializer)
{
	Field* f = malloc(sizeof(Field));
	f->modifiers = modifiers;
	f->type = type;
	f->name = strdup(name);
	f->initializer = initializer;
	return f;
}

// ======================================= Method Constructor =======================================

Method* create_method(int modifiers, Type* return_type, char* name, Parameter** params, int param_count, Statement* body)
{
	Method* m = malloc(sizeof(Method));
	m->modifiers = modifiers;
	m->return_type = return_type;
	m->name = strdup(name);
	m->params = params;
	m->param_count = param_count;
	m->body = body;
	return m;
}

// ======================================= Constructor Constructor =======================================

Constructor* create_constructor(int modifiers, Type* type, Parameter** params, int param_count, Statement* body)
{
	Constructor* c = malloc(sizeof(Constructor));
	c->modifiers = modifiers;
	c->type = type;
	c->params = params;
	c->param_count = param_count;
	c->body = body;
	return c;
}

// ======================================= Class Constructor =======================================

ClassDecl* create_class(char* name)
{
	ClassDecl* c = malloc(sizeof(ClassDecl));
	c->modifiers = MOD_PUBLIC;
	c->name = strdup(name);
	c->super_class = NULL;
	c->fields = NULL;
	c->field_count = 0;
	c->constructors = NULL;
	c->constructor_count = 0;
	c->methods = NULL;
	c->method_count = 0;
	return c;
}

void class_set_superclass(ClassDecl* cls, char* super_class)
{
	cls->super_class = strdup(super_class);
}

void class_set_modifiers(ClassDecl* class, int modifiers)
{
	class->modifiers = modifiers;
}

void class_add_field(ClassDecl* cls, Field* field)
{
	cls->fields = realloc(cls->fields, (cls->field_count + 1) * sizeof(Field*));
	cls->fields[cls->field_count] = field;
	cls->field_count++;
}

void class_add_constructor(ClassDecl* cls, Constructor* constructor)
{
	cls->constructors = realloc(cls->constructors, (cls->constructor_count + 1) * sizeof(Constructor*));
	cls->constructors[cls->constructor_count] = constructor;
	cls->constructor_count++;
}

void class_add_method(ClassDecl* cls, Method* method)
{
	cls->methods = realloc(cls->methods, (cls->method_count + 1) * sizeof(Method*));
	cls->methods[cls->method_count] = method;
	cls->method_count++;
}

// ======================================= Compilation Unit Constructor =======================================

File* create_file()
{
	File* f = malloc(sizeof(File));
	f->classes = NULL;
	f->class_count = 0;
	return f;
}

void file_add_class(File* file, ClassDecl* cls)
{
	file->classes = realloc(file->classes, (file->class_count + 1) * sizeof(ClassDecl*));
	file->classes[file->class_count] = cls;
	file->class_count++;
}

// === Calcul depth of AST ===

int max(int a, int b){
	return (a > b) ? a : b;
}

int depthCallExpr(CallExpr *callExpr){
	int maxArgDepth = depthExpression(callExpr->callee);
	for (int i = 0; i < callExpr->arg_count; i++){
		maxArgDepth = max(maxArgDepth, depthExpression(callExpr->args[i]));
	}
	return maxArgDepth + 1;
}

int depthNewExpr(NewExpr *newExpr){
	int maxArgDepth = 0;
	for (int i = 0; i < newExpr->arg_count; i++){
		maxArgDepth = max(maxArgDepth, depthExpression(newExpr->args[i]));
	}
	return maxArgDepth + 1;
}

int depthExpression(Expression* expr){
	switch (expr->kind){
		case EXPR_LITERAL:
			return 0;
		case EXPR_VARIABLE:
			return 0;
		case EXPR_BINARY_OP:
			return 1 + max(depthExpression(expr->binary.left), depthExpression(expr->binary.right));
		case EXPR_UNARY_OP:
			return 1 + depthExpression(expr->unary.operand);
		case EXPR_CALL:
			return depthCallExpr(&expr->call);
		case EXPR_INSTOF:
			return 1 + depthExpression(expr->instof.val);
		case EXPR_NEW:
			return depthNewExpr(&expr->new_expr);
		case EXPR_FIELD_ACCESS:
			return 1 + depthExpression(expr->field_access.object);
		case EXPR_CAST:
			return 1 + depthExpression(expr->cast.expr);
		default:
			return 0;
	}
}

int depthBlock(Block* block){
	int maxDepth = 0;
	for (int i = 0; i < block->count; i++){
		maxDepth = max(maxDepth, depthStatement(block->stmts[i]));
	}
	return maxDepth + 1;
}

int depthIfStmt(IfStmt* ifstmt){
	int maxDepth = max(depthExpression(ifstmt->condition), depthStatement(ifstmt->then_branch));
	int elseDepth = 0;
	if (ifstmt->else_branch != NULL){
		elseDepth = depthStatement(ifstmt->else_branch);
	}
	return 1 + max(maxDepth, elseDepth);
}

int depthForStmt(ForStmt* forstmt){
	int maxDepth = 0;
	if (forstmt->init != NULL){
		maxDepth = depthStatement(forstmt->init);
	}
	if (forstmt->condition != NULL){
		maxDepth = max(maxDepth, depthExpression(forstmt->condition));
	}
	if (forstmt->increment != NULL){
		maxDepth = max(maxDepth, depthExpression(forstmt->increment));
	}
	maxDepth = max(maxDepth, depthStatement(forstmt->body));
	return 1 + maxDepth;;
}

int depthVarDecl(VarDecl* varDecl){
	if (varDecl->initializer != NULL){
		return depthExpression(varDecl->initializer);
	}
	return 0;
}

int depthStatement(Statement* state){
	switch (state->kind){
		case STMT_BLOCK:
			return depthBlock(&state->block);
		case STMT_EXPR:
			return depthExpression(state->expr);
		case STMT_RETURN:
			if (state->return_val != NULL){
				return 1 + depthExpression(state->return_val);
			}
			return 0;
		case STMT_FOR:
			return depthForStmt(&state->for_stmt);
		case STMT_IF:
			return depthIfStmt(&state->if_stmt);
		case STMT_VAR_DECL:
			return depthVarDecl(&state->var_decl);
		default:
			return 0;
	}
}

int depthField(Field* field){
	if (field->initializer != NULL){
		return 1 + depthExpression(field->initializer);
	}
	return 1;
}

int depthMethod(Method* meth){
	return 1 + depthStatement(meth->body);
}

int depthConstructor(Constructor* constr){
	return 1 + depthStatement(constr->body);
}


int depthClassDecl(ClassDecl* class){
	int maxDepth = 0;
	for (int i = 0; i < class->field_count; i++){
		maxDepth = max(maxDepth, depthField(class->fields[i]));
	}
	for (int i = 0; i < class->constructor_count; i++){
		maxDepth = max(maxDepth, depthConstructor(class->constructors[i]));
	}
	for (int i = 0; i < class->method_count; i++){
		maxDepth = max(maxDepth, depthMethod(class->methods[i]));
	}
	return 1 + maxDepth;
}

int depthFile(File* file){
	int maxDepth = 0;
	for (int i = 0; i < file->class_count; i++){
		maxDepth = max(maxDepth, depthClassDecl(file->classes[i]));
	}
	return 1 + maxDepth;
}

// ======================================= Freeing =======================================

void freeType(Type* type)
{
	if (type == NULL) return;
	switch (type->kind)
	{
	case TYPE_CLASS:
		free(type->class_name);
		break;
	default:
		break;
	}
	free(type);
}

void freeLiteral(Literal l)
{
	if (l.kind == LIT_STRING) free(l.string_val);
	return;
}

void freeCallExpr(CallExpr);
void freeExpression(Expression* expr)
{
	if (expr == NULL) return;

	switch (expr->kind)
	{
	case EXPR_LITERAL:
		freeLiteral(expr->literal);
		freeType(expr->type);
		break;
	case EXPR_VARIABLE:
		free(expr->var_name);
		freeType(expr->type);
		break;
	case EXPR_BINARY_OP:
		freeExpression(expr->binary.left);
		freeExpression(expr->binary.right);
		freeType(expr->type);
		break;
	case EXPR_UNARY_OP:
		freeExpression(expr->unary.operand);
		freeType(expr->type);
		break;
	case EXPR_CALL:
		freeCallExpr(expr->call);
		freeType(expr->type);
		break;
	case EXPR_INSTOF:
		freeExpression(expr->instof.val);
		freeType(expr->instof.comp);
		freeType(expr->type);
		break;
	case EXPR_NEW:
		freeNew(expr->new_expr);
		freeType(expr->type);
		break;
	case EXPR_FIELD_ACCESS:
		free(expr->field_access.field_name);
		freeExpression(expr->field_access.object);
		freeType(expr->type);
		break;
	case EXPR_CAST:
		freeType(expr->cast.target_type);
		freeExpression(expr->cast.expr);
		break;
	}
	free(expr);
}

void freeNew(NewExpr n)
{
	while(n.arg_count-- > 0)
		freeExpression(n.args[n.arg_count]);
	if (n.args) free(n.args);
}

void freeCallExpr(CallExpr cexpr)
{
	freeExpression(cexpr.callee);
	while(cexpr.arg_count-- > 0)
		freeExpression(cexpr.args[cexpr.arg_count]);
	if (cexpr.args) free(cexpr.args);
}

void freeStatement(Statement*);

void freeBlock(Block blk)
{
	while (blk.count-- > 0)
		freeStatement(blk.stmts[blk.count]);	
	if (blk.stmts) free(blk.stmts);
}

void freeFor(ForStmt forstmt)
{
	freeStatement(forstmt.init);
	freeExpression(forstmt.condition);
	freeExpression(forstmt.increment);
	freeStatement(forstmt.body);
}

void freeIf(IfStmt ifstmt)
{
	freeExpression(ifstmt.condition);
	freeStatement(ifstmt.then_branch);
	freeStatement(ifstmt.else_branch);
}

void freeVarDecl(VarDecl var)
{
	freeType(var.type);
	free(var.name);
	freeExpression(var.initializer);
}

void freeStatement(Statement* state)
{
	if (state == NULL) return;
	switch (state->kind)
	{
	case STMT_BLOCK:
		freeBlock(state->block);
		break;
	case STMT_EXPR:
		freeExpression(state->expr);
		break;
	case STMT_RETURN:
		freeExpression(state->return_val);
		break;
	case STMT_FOR:
		freeFor(state->for_stmt);
		break;
	case STMT_IF:
		freeIf(state->if_stmt);
		break;
	case STMT_VAR_DECL:
		freeVarDecl(state->var_decl);
		break;
	case STMT_BREAK:
	case STMT_CONTINUE:
	default:
		break;
	}
	free(state);
}

void freeParameter(Parameter* param)
{
	free(param->name);
	freeType(param->type);
	free(param);
}


void freeField(Field* field)
{
	free(field->name);
	freeType(field->type);
	freeExpression(field->initializer);
	free(field);
}

void freeConstructor(Constructor* constr)
{
	freeType(constr->type);

	while (constr->param_count-- > 0)
		freeParameter(constr->params[constr->param_count]);
	if (constr->params) free(constr->params);
	freeStatement(constr->body);
	free(constr);
}


void freeMethod(Method* meth)
{
	free(meth->name);

	freeType(meth->return_type);
	while (meth->param_count-- > 0)
		freeParameter(meth->params[meth->param_count]);
	if (meth->params) free(meth->params);

	freeStatement(meth->body);
	free(meth);
}

void freeClass(ClassDecl* class)
{
	free(class->name);
	if (class->super_class) free(class->super_class);

	while (class->field_count-- > 0)
		freeField(class->fields[class->field_count]);
	if (class->fields) free(class->fields);

	while (class->constructor_count-- > 0)
		freeConstructor(class->constructors[class->constructor_count]);
	if (class->constructors) free(class->constructors);

	while (class->method_count-- > 0)
		freeMethod(class->methods[class->method_count]);
	if (class->methods) free(class->methods);

	free(class);
}

void freeFile(File* file)
{
	while (file->class_count-- > 0)
		freeClass(file->classes[file->class_count]);
	if (file->classes) free(file->classes);

	free(file);
}
