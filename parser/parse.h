#ifndef PARSE2_H
#define PARSE2_H

#include <parser/actions/actions.h>
#include <parser/backend_parser.h>
#include <error/print_error.h>

#include <stdio.h>
#include <stdbool.h>

/**
 * reurns a AST for JURA
*/
AST* parse_main(Lexer* lex, int debug);

#endif
