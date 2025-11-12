#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "lexer.h"
#include "id_kw.h"
#include "string_lex.h"
#include "error/print_error.h"


// Initialize a lexer with source code
Lexer* lexer_init(char* src, int len)
{
	Lexer* lex = malloc(sizeof(Lexer));
	lex->source = src;
	lex->length = len;
	lex->pos = 0;
	lex->tok.type = TOKEN_EOF;
	lex->tok.position = 0;
	lexer_next(lex);
	return lex;
}

// Peek at current character without consuming
 char peek(Lexer* lex)
{
	if (lex->pos >= lex->length) {
		return '\0';
	}
	return lex->source[lex->pos];
}

// Consume and return current character
char advance(Lexer* lex)
{
	if (lex->pos >= lex->length) {
		return '\0';
	}
	return lex->source[lex->pos++];
}

// Skip whitespace
void skip_whitespace(Lexer* lex)
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

	/* -------- On lit un Nombre ---------*/
	if (isdigit(c)) {
		while (lex->pos < lex->length && isdigit(peek(lex))) {
			advance(lex);
		}
		lex->tok.type = TOKEN_NUMBER;
		lex->tok.position = start; 
		return;
	}

	/* ---------On lit une lettre --------*/
	if (isalpha(c)) {
		Token tok = id_kw_class(lex, start);
		lex->tok = tok;
		return ;
	}

	/* ---------On lit un string ---------*/
	if (c == '\"') {
		if (good_string(lex)) {
			lex->tok.type = TOKEN_STR;
			lex->tok.position = start;
		}
		else {
			exit(0); //pour arrêter la fonction main qui lit le code source

		}
		return ;
		
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
		case '(':
			lex->tok.type = TOKEN_LPAR;
			break;
		case ')':
			lex->tok.type = TOKEN_RPAR;
			break;
		case '%':
			lex->tok.type= TOKEN_BINOP_MOD;
			break;
		case '{':
			lex->tok.type= TOKEN_LACC;
			break;
		case '}':
			lex->tok.type= TOKEN_RACC;
			break;
		case '[':
			lex->tok.type = TOKEN_LCRO;
			break;
		case ']':
			lex->tok.type = TOKEN_RCRO;
			break;
		case ';':
			lex-> tok.type=TOKEN_PTSVG;
			break;
		case '.':
			lex-> tok.type= TOKEN_PTS;
			break;
		case ',':
			lex->tok.type= TOKEN_VG;
			break;
		case '!':
			switch (peek(lex)){
			case '=':
				lex->tok.type= TOKEN_BINOP_DIFF;
				advance(lex);
				break;
			
			default:
				lex->tok.type=TOKEN_OPP;
				break;
			}
			break;
		case '>':
			switch (peek(lex)){
			case '=':
				lex->tok.type= TOKEN_BINOP_SUPEG;
				advance(lex);
				break;
			
			default:
				lex->tok.type=TOKEN_BINOP_SUP;
				break;
			}
			break;
		case '<':
			switch (peek(lex)){
			case '=':
				lex->tok.type= TOKEN_BINOP_INFEG;
				advance(lex);
				break;
			
			default:
				lex->tok.type=TOKEN_BINOP_INF;
				break;
			}
			break;
		case '=':
			switch (peek(lex)){
			case '=':
				lex->tok.type= TOKEN_BINOP_2EQUAL;
				advance(lex);
				break;
			
			default:
				lex->tok.type=TOKEN_BINOP_EQ;
				break;
			}
			break;
		case '|':
			switch (peek(lex)){
			case '|':
				lex->tok.type= TOKEN_BINOP_OR;
				advance(lex);
				break;
			
			default: {
				char msg[46];
				sprintf(msg, "Unexpected character '%c'. Did you mean || ?", c);
				error_print(lex, msg);

				exit(1);
			}
			}
			break;
		case '&':
			switch (peek(lex)){
			case '&':
				lex->tok.type= TOKEN_BINOP_AND;
				advance(lex);
				break;
			
			default: {
				char msg[46];
				sprintf(msg, "Unexpected character '%c'. Did you mean && ?", c);
				error_print(lex, msg);

				exit(1);
			}
			}
			break;
		case '/' :
			switch(peek(lex)) {
			case '/' :
				find_next_newline_or_eof(lex);
				lex->tok.type = TOKEN_COMMENT;
				break ;
			
			case '*' : 
				if (good_comment(lex)) {
					lex->tok.type = TOKEN_COMMENT;
					break;
				}
				else {
					exit(0);
				}
			default : {
				lex->tok.type = TOKEN_BINOP_DIV;
				break;
			}
			}
			break;

		default: {
			char msg[27];
			sprintf(msg, "Unexpected character '%c'", c);
			error_print(lex, msg);

			exit(1);
		}
	}
	lex->tok.position = start;
}

// Copy what the token is mached to and puts in buf (should only be used for identifiers and numbers)
void token_text(Lexer* lex, Token tok, char* buf, int bufsize)
{
	int start = tok.position;
	int end = start;

	if (tok.type == TOKEN_NUMBER) {
		while (end < lex->length && isdigit(lex->source[end])) {
			end++;
		}
	} else if (tok.type == TOKEN_NUMBER) {
		while (end < lex->length && isalnum(lex->source[end])) {
			end++;
		}
	}
	else {
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
		case TOKEN_BINOP_AND: return "AND";
		case TOKEN_BINOP_MOD: return "MOD";
		case TOKEN_BINOP_OR: return "OR";
		case TOKEN_BINOP_EQ: return "EQ";
		case TOKEN_BINOP_2EQUAL: return "2EQUAL";
		case TOKEN_BINOP_DIFF: return "DIFF";
		case TOKEN_BINOP_INF: return "INF";
		case TOKEN_BINOP_INFEG: return "INFEG";
		case TOKEN_BINOP_SUP: return "SUP";
		case TOKEN_BINOP_SUPEG: return "SUPEG";
		case TOKEN_LACC: return "LACC";
		case TOKEN_RACC: return "RACC";
		case TOKEN_LCRO: return "LCRO";
		case TOKEN_RCRO: return "RCRO";
		case TOKEN_PTS: return "PTS";
		case TOKEN_PTSVG: return "PTSVG";
		case TOKEN_VG: return "VG";
		case TOKEN_OPP: return "OPP";

		case TOKEN_STR : return "STRING";

		case TOKEN_CLASS: return "CLASS";
		case TOKEN_BOOLEAN:    return "BOOLEAN";
        case TOKEN_ELSE:       return "ELSE";
        case TOKEN_EXTENDS:    return "EXTENDS";
        case TOKEN_FALSE:      return "FALSE";
        case TOKEN_FOR:        return "FOR";
        case TOKEN_IF:         return "IF";
        case TOKEN_INSTANCEOF: return "INSTANCEOF";
        case TOKEN_INT:        return "INT";
        case TOKEN_NEW:        return "NEW";
        case TOKEN_NULL:       return "NULL";
        case TOKEN_PUBLIC:     return "PUBLIC";
        case TOKEN_RETURN:     return "RETURN";
        case TOKEN_STATIC:     return "STATIC";
        case TOKEN_THIS:       return "THIS";
        case TOKEN_TRUE:       return "TRUE";
        case TOKEN_VOID:       return "VOID";
        case TOKEN_ID: return "IDENTIFIER";
        
		default: return "UNKNOWN"; // shouldn't ever be used...
	}
}

// Pretty print a token :=)
void token_pprint(Token tok, Lexer* lex) // mabe source file???
{
	printf("Token: %-10s at pos %2d", 
				token_type_name(tok.type), 
				tok.position);
	if (tok.type == TOKEN_NUMBER) {
			printf(" (value: %d)", token_number(lex, tok));
	}
	printf("\n");
}
