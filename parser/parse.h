#ifndef RECURSIVE_H
#define RECURSIVE_H

#include <lexer/lexer.h>
#include <ast/ast.h>
#include <error/print_error.h>

File* parse_fichier(Lexer* lex);

#endif