#include <parser/parse.h>

#define BASE_SUITE_SIZE 	10 // base size of a *_suite list

static Token err = (Token) {.position = -1, .type = TOKENTYPE_SIZE};

static inline bool iserr(Token t) {return t.type == err.type || t.position == err.position;}

/**
 * matches the lexer position with the type of the token expected. Advances the token on
 * consumtion
 * @return the matched token
 * returns a token that satisfies iserr. 
 */
static Token match(Lexer* lex, TokenType* values)
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
static Token match_nofail(Lexer* lex, TokenType* values)
{
	Token t = lexer_giveToken(lex);

	for (; *values != TOKENTYPE_SIZE; values++)
		if (t.type == *values) {lexer_next(lex); return t;}
	error_under_token(lex->source, lex->tok, "unexpected token");
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


void parse_cro_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_LCRO, TOKENTYPE_SIZE});
	if (iserr(t)) return;
	match_nofail(lex, (TokenType[]) {TOKEN_RCRO, TOKENTYPE_SIZE});
}

Parameter** parse_params(Lexer* lex)
{

}

Parameter** parse_params_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKEN_RPAR, TOKEN_BOOLEAN, TOKEN_INT, TOKEN_VOID, TOKENTYPE_SIZE});
	if (iserr(t)) {
		// TODO: erreur
	}
	if (t.type == TOKEN_RPAR) {
		return NULL;
	}
	return parse_params(lex);
}

Statement** parse_stmt_list(Lexer* lex)
{

}

void parse_function_tail(Lexer* lex, ClassDecl* class, Type* type, bool isMeth)
{
	match_nofail(lex, (TokenType[]) {TOKEN_LPAR, TOKENTYPE_SIZE});
	Parameter** params = parse_params(lex);
	match_nofail(lex, (TokenType[]) {TOKEN_RPAR, TOKENTYPE_SIZE});
	match_nofail(lex, (TokenType[]) {TOKEN_LACC, TOKENTYPE_SIZE});
	Statement** body = parse_stmt_list(lex);
	match_nofail(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});
	if (isMeth) {

	}
}

void parse_const_or_meth(Lexer* lex, ClassDecl* class, bool is_stat)
{

}

void parse_meth_or_attr(Lexer* lex, ClassDecl* class, Type* type, char* name)
{

}

void parse_decl_public(Lexer* lex, ClassDecl* class)
{
	Token t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return parse_const_or_meth(lex, class, true);
	}
	parse_const_or_meth(lex, class, false);
}

void parse_const_or_meth_or_attr(Lexer* lex, ClassDecl* class, Type* type)
{
	Token t = match_nofail(lex, (TokenType[]) {TOKEN_ID, TOKEN_LACC, TOKENTYPE_SIZE});
	if (t.type == TOKEN_ID) {
		char name[32]; token_text(lex, t, name, 32);
		parse_meth_or_attr(lex, class, type, name);
	}
	if (t.type == TOKEN_LACC) {
		// we are on a constructor
		parse_function_tail(lex, class, type, false);
	}
}

void parse_decl(Lexer* lex, ClassDecl* class)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PUBLIC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		parse_decl_public(lex, class);
		return;
	}
	t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		parse_const_or_meth(lex, class, false);
		return;
	}
	Type* type = parse_type(lex);
	parse_const_or_meth_or_attr(lex, class, type);
}

void parse_decl_list(Lexer* lex, ClassDecl* class)
{
	Token t = match(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});
	if (!iserr(t)) {
		return;
	}
	parse_decl(lex, class);
	parse_decl_list(lex, class);
}

bool parse_public_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_PUBLIC, TOKENTYPE_SIZE});
	return !iserr(t);	// on error we don't have a public
}

bool parse_static_opt(Lexer* lex)
{
	Token t = match(lex, (TokenType[]) {TOKEN_STATIC, TOKENTYPE_SIZE});
	return !iserr(t);	// on error we don't have a static
}

Token parse_extends_opt(Lexer* lex)
{
	match_nofail(lex, (TokenType[]) {TOKEN_EXTENDS, TOKENTYPE_SIZE});
	Token t = match(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKENTYPE_SIZE});
	return t;
}

ClassDecl* parse_class(Lexer* lex)
{
	bool pub = parse_public_opt(lex);
	bool stat = parse_static_opt(lex);
	match_nofail(lex, (TokenType[]) {TOKEN_CLASS, TOKENTYPE_SIZE});
	
	Token type_name = match(lex, (TokenType[]) {TOKEN_ID_TYPE, TOKENTYPE_SIZE});
	
	Token extends = parse_extends_opt(lex);

	char buff[32]; token_text(lex, type_name, buff, 32);
	ClassDecl* class = create_class(buff);

	token_text(lex, extends, buff, 32);
	class_set_superclass(class, buff);

	class_set_modifiers(class, pub*MOD_PUBLIC & stat*MOD_STATIC);

		
	match_nofail(lex, (TokenType[]) {TOKEN_LACC, TOKENTYPE_SIZE});

	parse_decl_list(lex, class);

	match_nofail(lex, (TokenType[]) {TOKEN_RACC, TOKENTYPE_SIZE});

	return class;
}

void parse_class_list(Lexer* lex, File* file)
{
	Token t = match(lex, (TokenType[]) {TOKEN_EOF, TOKENTYPE_SIZE});
	if (iserr(t)) {
		ClassDecl* class = parse_class(lex);
		file_add_class(file, class);
		parse_class_list(lex, file);
	}
}

File* parse_fichier(Lexer* lex)
{
	File* f = create_file();
	parse_class_list(lex, f);
	return f;
}
