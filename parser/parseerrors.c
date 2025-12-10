#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include "parseerrors.h"
char* err_table[RULE_SIZE][TOKENTYPE_SIZE];
/*
"Votre compilateur ne doit pas s’arrêter après une erreur lexicale ou syntaxique détectée, mais tenter de poursuivre
l’analyse syntaxique."
Méthode Panic-Mode Recovery : 
pop la stack jusqu'à au choix : 
    dernier point virgule (de la ligne précédente)
    left accolade
puis continuer analyse à ligne suivante
Pour ce qui est de err_table : 
    remplir seulement les cases où il y a des erreurs. Pour les cases valides de la SD, on part 
    du principe qu'on y accèdera jamais à cause de la ligne dans parse.c : 
        if (prod.exp.count == 0 && prod.action == NULL) {
*/
void panicmode(Lexer *lex, Stack *stk) {
    NUT curr = stack_pop(stk);
    while (curr.type != TOKEN_PTSVG || curr.type != TOKEN_LACC) {
        curr = stack_pop(stk);
    }
    advancetonextline(lex);
}
void init_err_table(void) {
    init_ADDrule_err();
    init_ADDSUITErule_err();
    init_MULTrule_err();
    init_MULTSUITErule_err();
    init_UNARYrule_err();
    init_COPrule_err();
    init_POSTFIXEXPRrule_err();
    init_POSTFIXTAILrule_err();
    init_PRIMARYrule_err();
}
void print_syntax_error(Lexer *lex, Rule rule, TokenType tok_type) {
    char *msg = err_table[rule][tok_type];
    error_print(lex, msg); //included from print_error.c
}







void init_ADDrule_err(void){
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[ADD][tok] = "Expected a parenthesis, an integer, or unary operator";
    }
}
void init_ADDSUITErule_err(void) {
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {

        err_table[ADD_SUITE][tok] = "Expected an operator";
    }
    err_table[ADD_SUITE][TOKEN_BINOP_EQ] = "Did you mean \"==\" ?";
    err_table[ADD_SUITE][TOKEN_OPP] = "Did you mean \"!=\" ?";
}
void init_MULTrule_err(void) {
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[MULT][tok] = "Expected a parenthesis, an integer, or unary operator";
    }
}
void init_MULTSUITErule_err(void) {
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[MULT_SUITE][tok] = "Expected an operator";
    }
    err_table[MULT_SUITE][TOKEN_BINOP_EQ] = "Did you mean \"==\" ?";
    err_table[MULT_SUITE][TOKEN_OPP] = "Did you mean \"!=\" ?";
}
void init_UNARYrule_err(void) {
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[UNARY][tok] = "Expected - or + or ! or modulo (from UNARY rule)";
    }
}
void init_COPrule_err(void) {
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[C_O_P][tok] = "Did not expect this token (from COP rule)";
    }
}
void init_POSTFIXEXPRrule_err(void) {
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[POSTFIX_EXPR][tok] = "Did not expect this token (from POSTFIXEXPR)";
    }
}
void init_POSTFIXTAILrule_err(void) {
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[POSTFIX_TAIL][tok] = "Did not expect this token (from POSTFIXTAIL)";
    }
}
void init_PRIMARYrule_err(void) {  
    for (int tok=0; tok < TOKENTYPE_SIZE; tok++) {
        err_table[PRIMARY][tok] = "Exepected a primitive type (integer, string, boolean, this, new, null)";
    }
}