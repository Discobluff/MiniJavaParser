#include "lexer/lexer.h"
#include "demo_parser/parse.h"	// TODO: change when implementation change
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void error_print(Lexer* lex, const char* message)
{
	int line_start = lex->pos;
	int line_end = lex->pos;
	int line_num = 1;

	while (line_start > 0 && lex->source[line_start - 1] != '\n') {
		line_start--;
	}

	for (int i = 0; i < line_start; i++)
		if (lex->source[i] == '\n')
			line_num++;

	while (line_end < lex->length && lex->source[line_end] != '\n')
		line_end++;

	int col = lex->pos - line_start + 1;

	fprintf(stderr, "\x1B[31mError\x1B[0m at line %d, column %d: \x1B[31m%s\x1B[0m\n", line_num, col, message);

	fprintf(stderr, "  | ");
	for (int i = line_start; i < line_end; i++) {
		fputc(lex->source[i], stderr);
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  |");
	for (int i = line_start; i < lex->pos; i++) {
		if (lex->source[i] == '\t') {
			fputc('\t', stderr);
		} else {
			fputc(' ', stderr);
		}
	}
	fprintf(stderr, "\x1B[31m^\x1B[0m\n");
}

void error_under_token(char* src, Token tok, const char* message)
{
	int len = strlen(src);

	Lexer* lex = lexer_init(src, len);

	lex->pos = tok.position;
	lex->tok = tok;

	if (tok.type == TOKEN_EOF) {
		error_print(lex, message);
		return;
	}

	lexer_next(lex);
		
	int token_size = lex->pos - tok.position;

	int line_start = tok.position;
	int line_end = tok.position;
	int line_num = 1;

	while (line_start > 0 && src[line_start - 1] != '\n') {
		line_start--;
	}

	for (int i = 0; i < line_start; i++)
		if (lex->source[i] == '\n')
			line_num++;

	while (line_end < len && src[line_end] != '\n')
		line_end++;

	int col = tok.position - line_start + 1;

	char buf[32];
	token_text(lex, tok, buf, sizeof(buf));
	fprintf(stderr, "\x1B[31mError\x1B[0m at line %d, column %d: \x1B[31m\"%s\"\x1B[0m\n", line_num, col, message);

	fprintf(stderr, "  | ");
	for (int i = line_start; i < line_end; i++) {
		fputc(lex->source[i], stderr);
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  | ");
	for (int i = line_start; i < tok.position; i++) {
		if (lex->source[i] == '\t') {
			fputc('\t', stderr);
		} else {
			fputc(' ', stderr);
		}
	}
	
	fprintf(stderr, "\x1B[31m^");
	for(int i=1; i<token_size; i++)
		fprintf(stderr, "~");
	fprintf(stderr, "\x1B[0m\n\n");
}

void parser_error(const char* message)
{
	fprintf(stderr, "\x1B[31m%s\x1B[0m\n", message);
}
