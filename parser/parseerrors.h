#ifndef PARSEERR_H
#define PARSERR_H

#include <parser/parse.h>
#include <lexer/lexer.h>
#include <error/print_error.h>

void init_err_table(void);
void panicmode(Lexer *lex, Stack *stk);
void print_syntax_error(Lexer *lex, Rule rule, TokenType tok_type);

void init_ADDrule_err(void);
void init_ADDSUITErule_err(void);
void init_MULTrule_err(void);
void init_MULTSUITErule_err(void);
void init_UNARYrule_err(void);
void init_COPrule_err(void);
void init_POSTFIXEXPRrule_err(void);
void init_POSTFIXTAILrule_err(void);
void init_PRIMARYrule_err(void);

#endif