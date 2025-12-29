#include <parser/parse.h>

#define BASE_SUITE_SIZE 	10 // base size of a *_suite list

Token err = (Token) {.position = -1, .type = TOKENTYPE_SIZE};

bool iserr(Token t) {return t.type == err.type || t.position == err.position;}

/**
 * matches the lexer position with the type of the token expected. Advances the token on
 * consumtion
 * @return the matched token
 * returns a token that satisfies iserr. 
 */
Token match(Lexer* lex, TokenType* values)
{
	Token t = lexer_giveToken(lex);

	for (; *values != TOKENTYPE_SIZE; values++)
		if (t.type == *values) {lexer_next(lex); return t;}
	return err;
}

Token match_without_consuming(Lexer* lex, TokenType* values)
{
	Token t = lexer_giveToken(lex);

	for (; *values != TOKENTYPE_SIZE; values++)
		if (t.type == *values) {return t;}
	return err;
}

/**
 * matches the lexer position with the type of the token expected. (Not for epsilon transitions)
 * @return the matched token
 * On error prints an error under the token.
 */
Token match_nofail(Lexer* lex, TokenType* values, const char* msg)
{
	Token t = lexer_giveToken(lex);

	for (; *values != TOKENTYPE_SIZE; values++)
		if (t.type == *values) {lexer_next(lex); return t;}
	printf("%s\n", token_type_name(t.type));
	error_under_token(lex->source, lex->tok, msg);
	exit(0);
}

Statement** parse_stmt_list(Lexer* lex, int* stmts_count);
Expression* parse_assignement(Lexer* lex);
Expression* parse_expr(Lexer* lex);
Expression* parse_cast_or_postfix(Lexer* lex);
Expression** parse_lexpr_opt(Lexer* lex, int* count);
Statement* parse_stmt_if(Lexer* lex, Expression* condition, Statement* then_branch);


// type -> "ident_type" .
// type -> "boolean" .
// type -> "int" .
// type -> "void" .
Type* parse_type(Lexer* lex)
{
	Token t = match_nofail(lex, 
		(TokenType[]) {TOKEN_ID_TYPE, TOKEN_BOOLEAN, TOKEN_INT, TOKEN_VOID, TOKENTYPE_SIZE}, 
		"unexpected token: expected a type"
	);
	switch (t.type)
	{
	case TOKEN_ID_TYPE: {
		char buff[32]; token_text(lex, t, buff, 32);
		return create_class_type(buff);
	}
	case TOKEN_BOOLEAN: return create_type(TYPE_BOOLEAN);
	case TOKEN_INT: 	return create_type(TYPE_INT);
	case TOKEN_VOID: 	return create_type(TYPE_VOID);
	default:
		return NULL; // shuddap compiler
	}
}

// primary -> "integer" .
// primary -> "string" .
// primary -> "true" .
// primary -> "false" .
// primary -> "this" .
// primary -> "null" .
// primary -> "ident" .
// primary -> "new" "ident_type" "(" lexpr_opt ")" .
Expression* parse_primary(Lexer* lex)
{
	Token t = match_nofail(lex,
		(TokenType[]) {TOKEN_NUMBER, TOKEN_STR, TOKEN_TRUE, TOKEN_FALSE, TOKEN_THIS, TOKEN_NULL, TOKEN_ID, TOKEN_NEW, TOKENTYPE_SIZE},
		"unexpected token: expected a literal or an instanciation"
	);
	
	switch (t.type) {
		case TOKEN_NUMBER: {
			int val = token_number(lex, t);
			return create_literal_int(val);
		}
		case TOKEN_STR: {
			char str_val[1024];
			token_text(lex, t, str_val, 1024);
			return create_literal_string(str_val);
		}
		case TOKEN_TRUE:
			return create_literal_boolean(1);
		case TOKEN_FALSE:
			return create_literal_boolean(0);
		case TOKEN_THIS: {
			return create_variable("this");
		}
		case TOKEN_NULL:
			return create_literal_null();
		case TOKEN_ID: {
			char id_name[256];
			token_identifier(lex, t, id_name, 256);
			return create_variable(id_name);
		}
		case TOKEN_NEW: {
			// "new" "ident_type" "(" lexpr_opt ")"
			Token type_tok = match_nofail(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKENTYPE_SIZE}, "unexpected token: expected an object");
			char class_name[256];
			token_text(lex, type_tok, class_name, 256);
			Type* type = create_class_type(class_name);
			
			match_nofail(lex, (TokenType[]) {TOKEN_LPAR, TOKENTYPE_SIZE}, "unexpected token: expected (");
			int arg_count = 0;
			Expression** args = parse_lexpr_opt(lex, &arg_count);
			match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");
			
			return create_new(type, args, arg_count);
		}
		default:
			return NULL;
	}
}

// lexpr_expr -> .
// lexpr_expr -> "," lexpr .
void parse_lexpr_expr(Lexer* lex, Expression*** args, int* count, int* capacity)
{
	Token t = match(lex, (TokenType[]) {TOKEN_VG, TOKENTYPE_SIZE});
	
	if (iserr(t)) {
		return;
	}

	if (*count >= *capacity) {
		*capacity *= 2;
		*args = realloc(*args, *capacity * sizeof(Expression*));
	}

	(*args)[*count++] = parse_expr(lex);

	parse_lexpr_expr(lex, args, count, capacity);
}

// lexpr -> expr lexpr_expr .
Expression** parse_lexpr(Lexer* lex, int* count)
{
	int capacity = 4;
	Expression** args = malloc(capacity * sizeof(Expression*));
	*count = 0;
	
	// Parse first expression
	args[*count] = parse_expr(lex);
	(*count)++;
	
	parse_lexpr_expr(lex, &args, count, &capacity);
	return args;
}

// lexpr_opt -> lexpr .
// lexpr_opt -> .
Expression** parse_lexpr_opt(Lexer* lex, int* count)
{
	*count = 0;
	
	Token t = match_without_consuming(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return NULL;
	}
	
	return parse_lexpr(lex, count);
}

// postfix_tail -> "." "ident" postfix_tail .
// postfix_tail -> "(" lexpr_opt ")" postfix_tail .
// postfix_tail -> .
Expression* parse_postfix_tail(Lexer* lex, Expression* expr)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PTS, TOKEN_LPAR, TOKENTYPE_SIZE});
	
	if (iserr(t)) {
		return expr;
	}
	
	if (t.type == TOKEN_PTS) {
		Token ident = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE}, "unexpected token: expected an identifier");
		char field_name[256];
		token_identifier(lex, ident, field_name, 256);
		expr = create_dot(expr, field_name);
		return parse_postfix_tail(lex, expr);
	} else if (t.type == TOKEN_LPAR) {
		int arg_count = 0;
		Expression** args = parse_lexpr_opt(lex, &arg_count);
		match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");
		expr = create_call(expr, args, arg_count);
		return parse_postfix_tail(lex, expr);
	}
	
	return expr;
}

// postfix_expr -> primary postfix_tail .
Expression* parse_postfix_expr(Lexer* lex)
{
	Expression* expr = parse_primary(lex);
	return parse_postfix_tail(lex, expr);
}

// primary_par -> expr ")" .
Expression* parse_primary_par(Lexer* lex)
{
	Expression* expr = parse_expr(lex);
	match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");
	return parse_postfix_tail(lex, expr);
}

// cast_or_postfix_par -> type ")" cast_or_postfix .
// cast_or_postfix_par -> primary_par postfix_tail .
Expression* parse_cast_or_postfix_par(Lexer* lex)
{
	Token t = match_without_consuming(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKEN_BOOLEAN, TOKEN_INT, TOKEN_VOID, TOKENTYPE_SIZE});
	
	if (!iserr(t)) {
		Type* type = parse_type(lex);
		
		match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");
		Expression* expr = parse_cast_or_postfix(lex);
		return create_cast(type, expr);
	}
	
	return parse_primary_par(lex);
}

// cast_or_postfix -> "(" cast_or_postfix_par .
// cast_or_postfix -> postfix_expr .
Expression* parse_cast_or_postfix(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_LPAR, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return parse_cast_or_postfix_par(lex);
	}
	return parse_postfix_expr(lex);
}

// unary -> "!" unary .
// unary -> "-" unary .
// unary -> cast_or_postfix .
Expression* parse_unary(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_OPP, TOKEN_BINOP_SUB, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		UnaryOp op = (t.type == TOKEN_OPP) ? UNARY_NOT : UNARY_NEG;
		Expression* operand = parse_unary(lex);
		return create_unary_op(op, operand);
	}
	return parse_cast_or_postfix(lex);
}

// mult_suite -> "*" unary mult_suite .
// mult_suite -> "/" unary mult_suite .
// mult_suite -> "%" unary mult_suite .
// mult_suite -> .
Expression* parse_mult_suite(Lexer* lex, Expression* left)
{
	Token t = match(lex, (TokenType[]) {TOKEN_BINOP_MULT, TOKEN_BINOP_DIV, TOKEN_BINOP_MOD, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		BinaryOp op;
		switch (t.type) {
			case TOKEN_BINOP_MULT: op = OP_MUL; break;
			case TOKEN_BINOP_DIV:  op = OP_DIV; break;
			case TOKEN_BINOP_MOD:  op = OP_MOD; break;
			default: return left;
		}
		Expression* right = parse_unary(lex);
		left = create_binary_op(op, left, right);
		return parse_mult_suite(lex, left);
	}
	return left;
}

// mult_expr -> unary mult_suite .
Expression* parse_mult_expr(Lexer* lex)
{
	Expression* left = parse_unary(lex);
	return parse_mult_suite(lex, left);
}

// add_suite -> "+" mult_expr add_suite .
// add_suite -> "-" mult_expr add_suite .
// add_suite -> .
Expression* parse_add_suite(Lexer* lex, Expression* left)
{
	Token t = match(lex, (TokenType[]) {TOKEN_BINOP_ADD, TOKEN_BINOP_SUB, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		BinaryOp op = (t.type == TOKEN_BINOP_ADD) ? OP_ADD : OP_SUB;
		Expression* right = parse_mult_expr(lex);
		left = create_binary_op(op, left, right);
		return parse_add_suite(lex, left);
	}
	return left;
}

// add -> mult_expr add_suite .
Expression* parse_add(Lexer* lex)
{
	Expression* left = parse_mult_expr(lex);
	return parse_add_suite(lex, left);
}

// relational_expr_suite -> "<" add relational_expr_suite .
// relational_expr_suite -> "<=" add relational_expr_suite .
// relational_expr_suite -> ">" add relational_expr_suite .
// relational_expr_suite -> ">=" add relational_expr_suite .
// relational_expr_suite -> "instanceof" type relational_expr_suite .
// relational_expr_suite -> .
Expression* parse_relational_expr_suite(Lexer* lex, Expression* left)
{
	Token t = match(lex, (TokenType[]) {TOKEN_BINOP_INF, TOKEN_BINOP_INFEG, TOKEN_BINOP_SUP, TOKEN_BINOP_SUPEG, TOKEN_INSTANCEOF, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		if (t.type == TOKEN_INSTANCEOF) {
			Type* type = parse_type(lex);
			left = create_instaceof(left, type);
			return parse_relational_expr_suite(lex, left);
		} else {
			BinaryOp op;
			switch (t.type) {
				case TOKEN_BINOP_INF:   op = OP_LT;  break;
				case TOKEN_BINOP_INFEG: op = OP_LTE; break;
				case TOKEN_BINOP_SUP:   op = OP_GT;  break;
				case TOKEN_BINOP_SUPEG: op = OP_GTE; break;
				default: return left;
			}
			Expression* right = parse_add(lex);
			left = create_binary_op(op, left, right);
			return parse_relational_expr_suite(lex, left);
		}
	}
	return left;
}

// relational_expr -> add relational_expr_suite .
Expression* parse_relational_expr(Lexer* lex)
{
	Expression* left = parse_add(lex);
	return parse_relational_expr_suite(lex, left);
}

// equality_expr_suite -> "==" relational_expr equality_expr_suite .
// equality_expr_suite -> "!=" relational_expr equality_expr_suite .
// equality_expr_suite -> .
Expression* parse_equality_expr_suite(Lexer* lex, Expression* left)
{
	Token t = match(lex, (TokenType[]) {TOKEN_BINOP_2EQUAL, TOKEN_BINOP_DIFF, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		BinaryOp op = (t.type == TOKEN_BINOP_2EQUAL) ? OP_EQ : OP_NEQ;
		Expression* right = parse_relational_expr(lex);
		left = create_binary_op(op, left, right);
		return parse_equality_expr_suite(lex, left);
	}
	return left;
}

// equality_expr -> relational_expr equality_expr_suite .
Expression* parse_equality_expr(Lexer* lex)
{
	Expression* left = parse_relational_expr(lex);
	return parse_equality_expr_suite(lex, left);
}

// land_expr_suite -> "&&" equality_expr land_expr_suite .
// land_expr_suite -> .
Expression* parse_land_expr_suite(Lexer* lex, Expression* left)
{
	Token t = match(lex, (TokenType[]) {TOKEN_BINOP_AND, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		Expression* right = parse_equality_expr(lex);
		left = create_binary_op(OP_AND, left, right);
		return parse_land_expr_suite(lex, left);
	}
	return left;
}

// land_expr -> equality_expr land_expr_suite .
Expression* parse_land_expr(Lexer* lex)
{
	Expression* left = parse_equality_expr(lex);
	return parse_land_expr_suite(lex, left);
}

// lor_expr_suite -> "||" land_expr lor_expr_suite .
// lor_expr_suite -> .
Expression* parse_lor_expr_suite(Lexer* lex, Expression* left)
{
	Token t = match(lex, (TokenType[]) {TOKEN_BINOP_OR, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		Expression* right = parse_land_expr(lex);
		left = create_binary_op(OP_OR, left, right);
		return parse_lor_expr_suite(lex, left);
	}
	return left;
}

// lor_expr -> land_expr lor_expr_suite .
Expression* parse_lor_expr(Lexer* lex)
{
	Expression* left = parse_land_expr(lex);
	return parse_lor_expr_suite(lex, left);
}

// expr -> lor_expr .
Expression* parse_expr(Lexer* lex)
{
	return parse_lor_expr(lex);
}

// expr_opt -> expr .
// expr_opt -> .
Expression* parse_expr_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PTSVG, TOKEN_RACC, TOKENTYPE_SIZE});
	Expression* expr;
	if (iserr(t))
		expr = parse_expr(lex);
	else
		expr = NULL;

	return expr;
}

// block -> "{" stmt_list "}" .
Statement* parse_block(Lexer* lex)
{
	int cpt = 0;
	Statement** stmts = parse_stmt_list(lex, &cpt);
	return create_block(stmts, cpt);
}

// stmt_ident -> assignement ";" .
// stmt_ident -> postfix_tail ";" .
Expression* parse_stmt_ident(Lexer* lex, char* name)
{
	Token t = match_without_consuming(lex, (TokenType[]) {TOKEN_PTSVG, TOKEN_LACC, TOKEN_PTS, TOKENTYPE_SIZE});
	Expression* expr;
	if (!iserr(t)) {
		expr = parse_postfix_tail(lex, NULL);
	}
	else {
		expr = create_binary_op(
			OP_EQ,
			parse_assignement(lex),
			create_variable(name)
		);
	}
	match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE}, "unexpected token: expected ;");
	return expr;
}

// assignement -> "=" expr .
Expression* parse_assignement(Lexer* lex)
{
	match_nofail(lex, (TokenType[]) {TOKEN_BINOP_EQ, TOKENTYPE_SIZE}, "unexpected token: expected =");
	Expression* expr = parse_expr(lex);
	return expr;
}

// cro_opt -> "[" "]" .
// cro_opt -> .
void parse_cro_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_LCRO, TOKENTYPE_SIZE});
	if (iserr(t)) return;
	match_nofail(lex, (TokenType[]) {TOKEN_RCRO, TOKENTYPE_SIZE}, "unexpected token: expected ]");
}

// params_tail -> .
// params_tail -> "," params .
Parameter** parse_params_tail(Lexer* lex, int* param_cpt, Parameter** param_lst)
{
	Token t = match_without_consuming(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return param_lst;
	}

	match_nofail(lex, (TokenType[]) {TOKEN_VG, TOKENTYPE_SIZE}, "unexpected token: expected ,");

	param_lst = realloc(param_lst, (*param_cpt + 1) * sizeof(Parameter*));
	Type* type = parse_type(lex);
	Token ident = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE}, "unexpected token: expected an identifier");
	parse_cro_opt(lex);
	
	char name[32]; token_text(lex, ident, name, 32);
	Parameter* param = create_parameter(type, name);
	param_lst[*param_cpt] = param;
	(*param_cpt)++;
	return parse_params_tail(lex, param_cpt, param_lst);
}

// params_opt -> params .
// params_opt -> .
Parameter** parse_params(Lexer* lex, int* param_cpt)
{
	Type* type = parse_type(lex);
	Token ident = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE}, "unexpected token: expected an identifier");
	parse_cro_opt(lex);
	
	char name[32]; token_text(lex, ident, name, 32);
	Parameter* param = create_parameter(type, name);
	Parameter** param_lst = malloc(sizeof(Parameter*));
	param_lst[0] = param;
	(*param_cpt)++;
	return parse_params_tail(lex, param_cpt, param_lst);
}

Parameter** parse_params_opt(Lexer* lex, int* param_cpt)
{
	Token t = match_without_consuming(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return NULL;
	}
	return parse_params(lex, param_cpt);
}

// stmt -> ";" .
// stmt -> type "ident" assignement ";" .
// stmt -> "ident" stmt_ident .
// stmt -> "for" "(" expr_opt ";" expr_opt ";" expr_opt ")" stmt .
// stmt -> block .
// stmt -> "return" expr_opt ";" .
// stmt -> "if" "(" expr ")" block stmt_if .
Statement* parse_stmt(Lexer* lex)
{
	Token t = lexer_giveToken(lex);
	switch (t.type)
	{
	case TOKEN_RACC : return NULL;
	case TOKEN_BOOLEAN:
	case TOKEN_INT:
	case TOKEN_VOID:
	case TOKEN_ID_TYPE: {
		Type* t = parse_type(lex);
		Token id = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE}, "unexpected token: expected an identifier");
		char name[32]; token_text(lex, id, name, 32);
		Expression* expr = parse_assignement(lex);
		return create_var_decl(
			t,
			name,
			expr
		);
	}
	case TOKEN_LACC: return parse_block(lex);
	case TOKEN_ID: {
		Token id = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE}, "unexpected token: expected an identifier");
		char name[32]; token_text(lex, id, name, 32);
		Expression* expr = parse_stmt_ident(lex, name);
		return create_expr_stmt(expr);
	}
	case TOKEN_PTSVG: return NULL;
	case TOKEN_FOR: {
		match(lex, (TokenType[]) {TOKEN_FOR, TOKENTYPE_SIZE});
		match_nofail(lex, (TokenType[]) {TOKEN_LPAR, TOKENTYPE_SIZE}, "unexpected token: expected (");
		Statement* init = parse_stmt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE}, "unexpected token: expected ;");
		Expression* cond = parse_expr_opt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE}, "unexpected token: expected ;");
		Expression* incr = parse_expr_opt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE}, "unexpected token: expected ;");
		Statement* stmt = parse_stmt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");
		return create_for_stmt(
			init,
			cond,
			incr,
			stmt
		);
	}
	case TOKEN_RETURN: {
		match(lex, (TokenType[]) {TOKEN_RETURN, TOKENTYPE_SIZE});
		Expression* expr = parse_expr_opt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE}, "unexpected token: expected ;");
		return create_return_stmt(
			expr
		);
	}
	case TOKEN_IF: {
		match(lex, (TokenType[]) {TOKEN_IF, TOKENTYPE_SIZE});
		match_nofail(lex, (TokenType[]) {TOKEN_LPAR, TOKENTYPE_SIZE}, "unexpected token: expected (");
		Expression* cond = parse_expr(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");
		Statement* then_branch = parse_stmt(lex);
		return parse_stmt_if(lex, cond, then_branch);
	}
	default:
		return NULL;
	}
}

// stmt_if -> "else" stmt .
// stmt_if -> .
Statement* parse_stmt_if(Lexer* lex, Expression* condition, Statement* then_branch)
{
	Token t = match(lex, (TokenType[]) {TOKEN_ELSE, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		Statement* else_branch = parse_stmt(lex);
		return create_if_stmt(condition, then_branch, else_branch);
	}
	return create_if_stmt(condition, then_branch, NULL);
}

// stmt_list -> stmt stmt_list .
// stmt_list -> .
Statement** parse_stmt_list(Lexer* lex, int* stmts_count)
{
	Statement** stmt_list = malloc(sizeof(Statement*));
	Token t = lexer_giveToken(lex);
	while (t.type != TOKEN_RACC) {
		(*stmts_count)++;
		stmt_list = realloc(stmt_list, *stmts_count*sizeof(Statement*));

		Statement* stmt = parse_stmt(lex);
		stmt_list[*stmts_count - 1] = stmt;
		t = lexer_giveToken(lex);
	}
	if (*stmts_count == 0) {
		free(stmt_list);
		return NULL;
	}
	return stmt_list;
}

// function_tail -> "(" params_opt ")" "{" stmt_list "}" .
void parse_function_tail(Lexer* lex, ClassDecl* class, Type* type, char* name, int modifier, bool is_constr)
{
	int params_count = 0;
	int stmts_count = 0;

	match_nofail(lex, (TokenType[]) {TOKEN_LPAR, TOKENTYPE_SIZE}, "unexpected token: expected (");
	Parameter** params = parse_params_opt(lex, &params_count);
	match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");

	match_nofail(lex, (TokenType[]) {TOKEN_LACC, TOKENTYPE_SIZE}, "unexpected token: expected {");
	Statement** body_stmts = parse_stmt_list(lex, &stmts_count);
	Statement*  body = create_block(body_stmts, stmts_count);
	match_nofail(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE}, "unexpected token: expected }");

	if (is_constr) {
		// Constructeur
		if (type->kind != TYPE_CLASS) {
			error_print(lex, "no way this fails");
			exit(0);
		}
		Constructor* constr = create_constructor(
			modifier,
			type->class_name,
			params,
			params_count,
			body
		);
		class_add_constructor(class, constr);
	}
	else {
		// Method
		Method* meth = create_method(
			modifier,
			type,
			name,
			params,
			params_count,
			body
		);
		class_add_method(class, meth);
	}
}

// meth_or_attr -> function_tail .
// meth_or_attr -> ";" .
// meth_or_attr -> assignement ";" .
void parse_meth_or_attr(Lexer* lex, ClassDecl* class, Type* type, char* name)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		Field* feild = create_field(
			MOD_PUBLIC,
			type,
			name,
			NULL
		);
		class_add_field(class, feild);
		return;
	}

	t = lexer_giveToken(lex);
	if (t.type == TOKEN_LACC) {
		// on a Method
		return parse_function_tail(lex, class, type, name, MOD_PUBLIC, false);
	}
	if (t.type == TOKEN_BINOP_EQ) {
		// on an assignement
		Expression* expr = parse_assignement(lex);
		Field* field = create_field(
			MOD_PUBLIC,
			type,
			name,
			expr
		);
		class_add_field(class, field);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE}, "unexpected token: expected ;");
	}
	else {
		error_under_token(lex->source, t, "expected a method or assignement");
		exit(0);
	}
}

// const_or_meth_sol -> function_tail .
// const_or_meth_sol -> "ident" function_tail .
void parse_const_or_meth_sol(Lexer* lex, ClassDecl* class, int modifier, Type* type)
{
	Token ident = match(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
	if (!iserr(ident)) {
		char name[32]; token_text(lex, ident, name, 32);
		return parse_function_tail(lex, class, type, name, modifier, false);
	}
	parse_function_tail(lex, class, type, NULL, modifier, true);
}

// const_or_meth -> type const_or_meth_sol .
void parse_const_or_meth(Lexer* lex, ClassDecl* class, int modifier)
{
	Type* type = parse_type(lex);
	parse_const_or_meth_sol(lex, class, modifier, type);
}

// decl_public -> const_or_meth .
// decl_public -> "static" const_or_meth .
void parse_decl_public(Lexer* lex, ClassDecl* class)
{
	Token t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		parse_const_or_meth(lex, class, MOD_STATIC | MOD_PUBLIC);
	}
	parse_const_or_meth(lex, class, MOD_PUBLIC);
}

// const_or_meth_or_attr -> "ident" meth_or_attr .
// const_or_meth_or_attr -> function_tail .
void parse_const_or_meth_or_attr(Lexer* lex, ClassDecl* class, Type* type)
{
	Token t = match(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
	if (iserr(t)) {
		char name[32]; token_text(lex, t, name, 32);
		parse_meth_or_attr(lex, class, type, name);
	}
	// on a constructor
	parse_function_tail(lex, class, type, NULL, MOD_PUBLIC, true);
}

// decl -> "public" decl_public .
// decl -> "static" const_or_meth .
// decl -> type const_or_meth_or_attr .
void parse_decl(Lexer* lex, ClassDecl* class)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PUBLIC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return parse_decl_public(lex, class);
	}
	t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return parse_const_or_meth(lex, class, MOD_STATIC);
	}
	Type* type = parse_type(lex);
	parse_const_or_meth_or_attr(lex, class, type);
}

// decl_list -> decl decl_list .
// decl_list -> .
void parse_decl_list(Lexer* lex, ClassDecl* class)
{
	if (lex->tok.type == TOKEN_RACC) {
		return;
	}
	Token t = match_nofail(
		lex, 
		(TokenType[]) {TOKEN_LCRO, TOKEN_ID_TYPE, TOKEN_PUBLIC, TOKEN_STATIC, TOKEN_BOOLEAN, TOKEN_INT, TOKEN_VOID, TOKENTYPE_SIZE},
		"unexpected token: expected a declaration"
	);
	if (t.type == TOKEN_LCRO)
		return;
	parse_decl(lex, class);
	parse_decl_list(lex, class);
}

// public_opt -> "public" .
// public_opt -> .
bool parse_public_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PUBLIC, TOKENTYPE_SIZE});
	return !iserr(t);
}

// static_opt -> "static" .
// static_opt -> .
bool parse_static_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	return !iserr(t);
}

// classe -> public_opt static_opt "class" "ident_type" extends_opt "{" decl_list "}" .
ClassDecl* parse_class(Lexer* lex)
{
	bool is_pub = parse_public_opt(lex);
	bool is_stat = parse_static_opt(lex);

	match_nofail(lex, (TokenType[]) {TOKEN_CLASS, TOKENTYPE_SIZE}, "unexpected token: expected an identifier");
	Token ident = match_nofail(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKENTYPE_SIZE}, "unexpected token: expected an identifier");

	char buff[32]; token_text(lex, ident, buff, 32);
	ClassDecl* class = create_class(buff);
	class_set_modifiers(class, is_pub*MOD_PUBLIC | is_stat*MOD_STATIC);

	// extends_opt
	Token ext = match(lex, (TokenType[]) {TOKEN_EXTENDS, TOKENTYPE_SIZE});
	if (!iserr(ext)) {
		ext = match(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKENTYPE_SIZE});
		token_text(lex, ext, buff, 32);
		class_set_superclass(class, buff);
	}

	match_nofail(lex, (TokenType[]) {TOKEN_LACC, TOKENTYPE_SIZE}, "unexpected token: expected (");
	parse_decl_list(lex, class);
	match_nofail(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE}, "unexpected token: expected a closing parenthese");

	return class;
}

// class_list -> classe class_list .
// class_list -> .
void parse_class_list(Lexer* lex, File* file)
{
	Token t = match(lex, (TokenType[]) {TOKEN_EOF, TOKENTYPE_SIZE});
	if (!iserr(t)) return;

	ClassDecl* c = parse_class(lex);
	file_add_class(file, c);
	parse_class_list(lex, file);
}

// fichier -> class_list .
File* parse_file(Lexer* lex)
{
	File* f = create_file();
	parse_class_list(lex, f);
	return f;
}
