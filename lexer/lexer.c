#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


typedef enum {
	TOKEN_NUMBER,

	TOKEN_BINOP_ADD,
	TOKEN_BINOP_SUB,
	TOKEN_BINOP_MULT,
	TOKEN_BINOP_DIV,
	
	TOKEN_LPAR,
	TOKEN_RPAR,

	TOKEN_EOF,
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
typedef struct {
	TokenType type;			// Tagged union ;)
	int position;
} Token;


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


// Initialize a lexer with source code
Lexer* lexer_init(char* src, int len)
{
	Lexer* lex = malloc(sizeof(Lexer));
	lex->source = src;
	lex->length = len;
	lex->pos = 0;
	lex->tok.type = TOKEN_EOF;
	lex->tok.position = 0;
	return lex;
}

// Peek at current character without consuming
static char peek(Lexer* lex)
{
	if (lex->pos >= lex->length) {
		return '\0';
	}
	return lex->source[lex->pos];
}

// Consume and return current character
static char advance(Lexer* lex)
{
	if (lex->pos >= lex->length) {
		return '\0';
	}
	return lex->source[lex->pos++];
}

// Skip whitespace
static void skip_whitespace(Lexer* lex)
{
	while (lex->pos < lex->length && isspace(peek(lex))) {
		advance(lex);
	}
}

// Go to next token and put it in lex->tok
void lexer_next(Lexer* lex)
{
	skip_whitespace(lex);

	int start = lex->pos;

	if (lex->pos >= lex->length) {
		lex->tok.type = TOKEN_EOF;
		lex->tok.position = start;
		return;
	}

	char c = peek(lex);

	if (isdigit(c)) {
		while (lex->pos < lex->length && isdigit(peek(lex))) {
			advance(lex);
		}
		lex->tok.type = TOKEN_NUMBER;
		lex->tok.position = start;
		return;
	}

	advance(lex);
	switch (c) {
		case '+':
			lex->tok.type = TOKEN_BINOP_ADD;
			break;
		case '-':
			lex->tok.type = TOKEN_BINOP_SUB;
			break;
		case '*':
			lex->tok.type = TOKEN_BINOP_MULT;
			break;
		case '/':
			lex->tok.type = TOKEN_BINOP_DIV;
			break;
		case '(':
			lex->tok.type = TOKEN_LPAR;
			break;
		case ')':
			lex->tok.type = TOKEN_RPAR;
			break;
		default:
			fprintf(stderr, "Unexpected character '%c' at position %d\n", c, start);
			lex->tok.type = TOKEN_EOF;
			break;
	}
	lex->tok.position = start;
}

// Copy what the token is mached to and puts in buf
void token_text(Lexer* lex, Token tok, char* buf, int bufsize)
{
	int start = tok.position;
	int end = start;

	if (tok.type == TOKEN_NUMBER) {
		while (end < lex->length && isdigit(lex->source[end])) {
			end++;
		}
	} else {
		end = start + 1;
	}

	int len = end - start;
	if (len >= bufsize) len = bufsize - 1;

	for (int i = 0; i < len; i++) {
		buf[i] = lex->source[start + i];
	}
	buf[len] = '\0';
}

int token_number(Lexer* lex, Token tok)
{
	char buf[32];
	token_text(lex, tok, buf, sizeof(buf));
	return atoi(buf);
}

// Get token type name for debugging
const char* token_type_name(TokenType type)
{
	switch (type) {
		case TOKEN_NUMBER: return "NUMBER";
		case TOKEN_BINOP_ADD: return "ADD";
		case TOKEN_BINOP_SUB: return "SUB";
		case TOKEN_BINOP_MULT: return "MULT";
		case TOKEN_BINOP_DIV: return "DIV";
		case TOKEN_LPAR: return "LPAR";
		case TOKEN_RPAR: return "RPAR";
		case TOKEN_EOF: return "EOF";
		default: return "UNKNOWN";
	}
}

// Pretty print a token :=)
void token_pprint(Token tok, Lexer* lex) // mabe source file???
{
	printf("Token: %-8s at pos %2d", 
				token_type_name(tok.type), 
				tok.position);
	if (tok.type == TOKEN_NUMBER) {
			printf(" (value: %d)", token_number(lex, tok));
	}
	printf("\n");
}

int main(void)
{
	char source[] = "10 + 98";
	Lexer* lex = lexer_init(source, sizeof(source) - 1);

	printf("Lexing: \"%s\"\n\n", source);

	do {
		lexer_next(lex);
		token_pprint(lex->tok, lex);
	} while (lex->tok.type != TOKEN_EOF);

	free(lex);
	return 0;
}
