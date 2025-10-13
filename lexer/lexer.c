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


// TODO: what do we do with identifiers? (put them directely in the symbol table and pass them by index?))
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


char peek(Lexer* lex)
{
	if (lex->pos >= lex->length) return '\0';
	return lex->source[lex->pos];
}

// when we have the start and the type of a token we don't need the lexer to point to that value anymore
void lexer_pass_fodder(Lexer* lex)
{
	if (lex->tok.type == TOKEN_EOF) return ;

	switch (lex->tok.type)
	{
	case TOKEN_NUMBER:{
		int i = lex->tok.position;
		while(i < lex->length && isdigit(lex->source[i])) i++;
		lex->pos = i;
		break;
	}
	case TOKEN_EOF:
		break;
	default:
		lex->pos++;
		break;
	}
	return ;
}

// the type of the next token???
bool lexer_next(Lexer* lex)
{
	while(isspace(peek(lex))) lex->pos++;

	if(peek(lex) == '\0')
	{
		lex->tok.position = lex->length;
		lex->tok.type = TOKEN_EOF;
		return false;
	}

	char c = peek(lex);

	if (isdigit(c)) lex->tok.type = TOKEN_NUMBER;
	else if (c == '+') lex->tok.type = TOKEN_BINOP_ADD;
	else if (c == '-') lex->tok.type = TOKEN_BINOP_SUB;
	else if (c == '*') lex->tok.type = TOKEN_BINOP_MULT;
	else if (c == '/') lex->tok.type = TOKEN_BINOP_DIV;
	else if (c == '(') lex->tok.type = TOKEN_LPAR;
	else if (c == ')') lex->tok.type = TOKEN_RPAR;
	else {
		printf("Lexer error: unknown token '%c'\n", c);
		exit(1);
	}
	lex->tok.position = lex->pos;
	lexer_pass_fodder(lex);
	return true;
}


Lexer* lexer_create(const char* source)
{
	Lexer* lex = malloc(sizeof(Lexer));

	lex->pos = 0;
	lex->length = strlen(source);
	lex->source = malloc(lex->length + 1);
	strcpy(lex->source, source);
	lex->tok.position = 0;
	lex->tok.type = TOKEN_EOF;
	lexer_next(lex);

	return lex;
}

Token lexer_give_token(Lexer* lex)
{
	return lex->tok;
}

// à revoir... same problem then with identifiers in the tokens...
void print_token(Token tok, const char* source)
{
	printf("Token info: '");
	if (tok.type == TOKEN_NUMBER) {
		int pos = tok.position; int res = 0;
		while(pos < strlen(source) && isdigit(source[pos])) {
			res = 10*res + source[pos] - '0';
			pos++;
		}
		printf("%d", res);
	}
	else if (tok.type == TOKEN_BINOP_ADD)
		printf("+");
	else if (tok.type == TOKEN_BINOP_SUB)
		printf("-");
	else if (tok.type == TOKEN_BINOP_MULT)
		printf("*");
	else if (tok.type == TOKEN_BINOP_DIV)
		printf("/");
	else if (tok.type == TOKEN_RPAR)
		printf("(");
	else if (tok.type == TOKEN_LPAR)
		printf(")");
	else if (tok.type == TOKEN_EOF)
		printf("EOF");
	
	printf("' @ pos: %d\n", tok.position);
}

void lexer_free(Lexer *lex) 
{
	free(lex->source);
	free(lex);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <expression>\n", argv[0]);
        return 1;
    }
    
    // Concatenate all arguments into a single string
    size_t total_len = 0;
    for (int i = 1; i < argc; i++) {
        total_len += strlen(argv[i]) + 1; // +1 for space
    }
    
    char* src = malloc(total_len);
    
    src[0] = '\0';
    for (int i = 1; i < argc; i++) {
        strcat(src, argv[i]);
        if (i < argc - 1) {
            strcat(src, " ");
        }
    }
    
    Lexer* lexer = lexer_create(src);
    print_token(lexer_give_token(lexer), src);
    while (lexer_next(lexer)) {
        print_token(lexer_give_token(lexer), src);
    }
    print_token(lexer_give_token(lexer), src);
    lexer_free(lexer);
    free(src);
    
    return 0;
}