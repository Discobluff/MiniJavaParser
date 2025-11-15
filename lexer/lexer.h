#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

typedef enum {
	TOKEN_NUMBER,

	TOKEN_BINOP_ADD,
	TOKEN_BINOP_SUB,
	TOKEN_BINOP_MULT,
	TOKEN_BINOP_DIV,
	TOKEN_BINOP_MOD,
	TOKEN_BINOP_OR,
	TOKEN_BINOP_AND,
	TOKEN_BINOP_EQ,
	TOKEN_BINOP_2EQUAL,
	TOKEN_BINOP_DIFF,

	TOKEN_BINOP_INF,
	TOKEN_BINOP_SUP,
	TOKEN_BINOP_INFEG,
	TOKEN_BINOP_SUPEG,
	
	TOKEN_LPAR,
	TOKEN_RPAR,
    TOKEN_CLASS,
    TOKEN_KEYWORD,
    TOKEN_ID,

	TOKEN_LACC,
	TOKEN_RACC,
	TOKEN_LCRO,
	TOKEN_RCRO,

	TOKEN_PTSVG,
	TOKEN_VG,

	TOKEN_OPP,
	TOKEN_PTS,

	TOKEN_STR,
	TOKEN_COMMENT,

	TOKEN_EOF,

    TOKEN_BOOLEAN,
    TOKEN_ELSE,
    TOKEN_EXTENDS,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_INSTANCEOF,
    TOKEN_INT,
    TOKEN_NEW,
    TOKEN_NULL,
    TOKEN_PUBLIC,
    TOKEN_RETURN,
    TOKEN_STATIC,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VOID,

	TOKEN_UNOP_SUB,
	TOKEN_UNOP_ADD,

	// leave at the end
	TOKENTYPE_SIZE, 
} TokenType;


// TODO: what do we do with identifiers? (put them directely in the symbol table and pass them by index?)
/**
 * A token is a position of the start of the token and a beginning:
 * The content is recomputed on demand and casted to the right type
 * string for identifiers, integers for ints and string for strings
 * 
 * - position points to the begining of the identifier 
 * - the type is the type
 */
struct Token{
	TokenType type;			// Tagged union ;)
	int position;
} ;

typedef struct Token Token;

/** 
 * A Lexer is a position from which we have read all the preceding 
 * chars.
 */
typedef struct {
	char* source;
	int length;

	int pos;
	Token tok;
} Lexer;


/**
 * Internal
 */

char peek(Lexer* lex);
char advance(Lexer* lex);
void skip_whitespace(Lexer* lex);
const char* token_type_name(TokenType type);
void token_text(Lexer* lex, Token tok, char* buf, int bufsize);

/**
 * API
 */

Lexer* lexer_init(char* src, int len);
void lexer_next(Lexer* lex);
Token lexer_giveToken(Lexer* lex);
void token_pprint(Token tok, Lexer* lex);

void token_text(Lexer* lex, Token tok, char* buf, int bufsize);
int token_number(Lexer* lex, Token tok);
bool token_type_equality(TokenType tok1, Token tok2);

#endif