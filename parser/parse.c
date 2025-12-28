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

/**
 * matches the lexer position with the type of the token expected. (Not for epsilon transitions)
 * @return the matched token
 * On error prints an error under the token.
 */
Token match_nofail(Lexer* lex, TokenType* values)
{
	Token t = lexer_giveToken(lex);

	for (; *values != TOKENTYPE_SIZE; values++)
		if (t.type == *values) {lexer_next(lex); return t;}
	error_under_token(lex->source, lex->tok, "unexpected token (from match_nofail)");
	exit(0);
}


Type* parse_type(Lexer* lex)
{
	Token t = match_nofail(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKEN_BOOLEAN, TOKEN_INT, TOKEN_VOID, TOKENTYPE_SIZE});
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


Expression* parse_expr(Lexer* lex)
{

}

Expression* parse_expr_opt(Lexer* lex)
{
	
}

Expression* parse_block(Lexer* lex)
{

}

Expression* parse_stmt_ident(Lexer* lex, char* name)
{

}

Expression* parse_assignement(Lexer* lex)
{
	match_nofail(lex, (TokenType[]) {TOKEN_BINOP_EQ, TOKENTYPE_SIZE});
	Expression* expr = parse_expr(lex);
	return NULL;
}

void parse_cro_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_LCRO, TOKENTYPE_SIZE});
	if (iserr(t)) return;
	match_nofail(lex, (TokenType[]) {TOKEN_RCRO, TOKENTYPE_SIZE});
}

Parameter** parse_param_tail(Lexer* lex, int* param_cpt, Parameter** param_lst)
{
	Token t = match(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return param_lst;
	}

	match_nofail(lex, (TokenType[]) {TOKEN_VG, TOKENTYPE_SIZE});

	param_lst = realloc(param_lst, (*param_cpt + 1) * sizeof(Parameter*));
	Type* type = parse_type(lex);
	Token ident = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
	parse_cro_opt(lex);
	
	char name[32]; token_text(lex, ident, name, 32);
	Parameter* param = create_parameter(type, name);
	param_lst[*param_cpt] = param;
	*param_cpt++;
	return parse_param_tail(lex, param_cpt, param_lst);
}


Parameter** parse_params(Lexer* lex, int* param_cpt)
{
	Type* type = parse_type(lex);
	Token ident = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
	parse_cro_opt(lex);
	
	char name[32]; token_text(lex, ident, name, 32);
	Parameter* param = create_parameter(type, name);
	Parameter** param_lst = malloc(sizeof(Parameter*));
	param_lst[0] = param;
	*param_cpt++;
	return parse_param_tail(lex, param_cpt, param_lst);
}

Parameter** parse_params_opt(Lexer* lex, int* param_cpt)
{
	Token t = match(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return NULL;
	}
	return parse_params(lex, param_cpt);
}

Statement* parse_stmt(Lexer* lex)
{
	Token t = lexer_giveToken(lex);
	switch (t.type)
	{
	case TOKEN_BOOLEAN:
	case TOKEN_INT:
	case TOKEN_VOID:
	case TOKEN_ID_TYPE: {
		Type* t = parse_type(lex);
		Token id = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
		char name[32]; token_text(lex, id, name, 32);
		Expression* expr = parse_assignement(lex);
		return create_var_decl(
			t,
			name,
			expr
		);
	}
	case TOKEN_LACC: return parse_block(lex);
	case TOKEN_ID:{
		Token id = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
		char name[32]; token_text(lex, id, name, 32);
		return parse_stmt_ident(lex, name);
	}
	case TOKEN_PTSVG: return NULL;
	case TOKEN_FOR: {
		match_nofail(lex, (TokenType[]) {TOKEN_FOR, TOKENTYPE_SIZE});
		match_nofail(lex, (TokenType[]) {TOKEN_LACC, TOKENTYPE_SIZE});
		Statement* init = parse_stmt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE});
		Expression* cond = parse_expr_opt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE});
		Expression* incr = parse_expr_opt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE});
		Statement* stmt = parse_stmt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});
		return create_for_stmt(
			init,
			cond,
			incr,
			stmt
		);
	}
	case TOKEN_RETURN: {
		match_nofail(lex, (TokenType[]) {TOKEN_RETURN, TOKENTYPE_SIZE});
		Expression* expr = parse_expr_opt(lex);
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE});
		return create_return_stmt(
			expr
		);
	}
	case TOKEN_IF:
		// TODO: if case ...
	default:
		// TODO: error
		return NULL;
	}
}

Statement** parse_stmt_list(Lexer* lex, int* stmts_count)
{
	Statement** stmt_list = malloc(sizeof(Statement*));
	Token t = match(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});
	while (!iserr(t)) {
		*stmts_count++;
		stmt_list = realloc(stmt_list, *stmts_count*sizeof(Statement*));

		Statement* stmt = parse_stmt(lex);
		t = match(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});
	}
	return stmt_list;
}

void parse_function_tail(Lexer* lex, ClassDecl* class, Type* type, char* name, int modifier, bool is_constr)
{
	int params_count = 0;
	int stmts_count = 0;

	match_nofail(lex, (TokenType[]) {TOKEN_LPAR, TOKENTYPE_SIZE});
	Parameter** params = parse_params_opt(lex, &params_count);
	match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE});
	
	match_nofail(lex, (TokenType[]) {TOKEN_LACC, TOKENTYPE_SIZE});
	Statement** body_stmts = parse_stmt_list(lex, &stmts_count);
	Statement*  body = create_block(body_stmts, stmts_count);
	match_nofail(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});

	if (is_constr) {
		// Constructeur
		Constructor* constr = create_constructor(
			modifier,
			type->class_name,	//TOOD: this is a failure point (check if type is class)
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
		match_nofail(lex, (TokenType[]) {TOKEN_PTSVG, TOKENTYPE_SIZE});
	}
	else {
		// TODO: error
	}
}

void parse_const_or_meth_sol(Lexer* lex, ClassDecl* class, int modifier, Type* type)
{
	Token ident = match(lex, (TokenType[]) {TOKEN_ID, TOKENTYPE_SIZE});
	if (!iserr(ident)) {
		char name[32]; token_text(lex, ident, name, 32);
		return parse_function_tail(lex, class, type, name, modifier, false);
	}
	parse_function_tail(lex, class, type, NULL, modifier, true);
}

void parse_const_or_meth(Lexer* lex, ClassDecl* class, int modifier)
{
	Type* type = parse_type(lex);
	parse_const_or_meth_sol(lex, class, modifier, type);
}

void parse_decl_public(Lexer* lex, ClassDecl* class)
{
	Token t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		parse_const_or_meth(lex, class, MOD_STATIC | MOD_PUBLIC);
	}
	parse_const_or_meth(lex, class, MOD_PUBLIC);
}

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

void parse_decl_list(Lexer* lex, ClassDecl* class)
{
	Token t = match_nofail(
		lex, 
		(TokenType[]) {TOKEN_LCRO, TOKEN_ID_TYPE, TOKEN_PUBLIC, TOKEN_STATIC, TOKEN_BOOLEAN, TOKEN_INT, TOKEN_VOID, TOKENTYPE_SIZE}
	);
	if (t.type == TOKEN_LCRO)
		return;
	parse_decl(lex, class);
	parse_decl_list(lex, class);
}

bool parse_public_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PUBLIC, TOKENTYPE_SIZE});
	return !iserr(t);
}

bool parse_static_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	return !iserr(t);
}


ClassDecl* parse_class(Lexer* lex) // public static class Chien extends Animal { ... }
{
	bool is_pub = parse_public_opt(lex);
	bool is_stat = parse_static_opt(lex);

	match_nofail(lex, (TokenType[]) {TOKEN_CLASS, TOKENTYPE_SIZE});
	Token ident = match_nofail(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKENTYPE_SIZE});

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

	match_nofail(lex, (TokenType[]) {TOKEN_LACC, TOKENTYPE_SIZE});
	parse_decl_list(lex, class);
	match_nofail(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});

	return class;
}

void parse_class_list(Lexer* lex, File* file)
{
	ClassDecl* c = parse_class(lex);
	file_add_class(file, c);
	parse_class_list(lex, file);
}

File* parse_file(Lexer* lex)
{
	File* f = create_file();
	parse_class_list(lex, f);
	return f;
}
