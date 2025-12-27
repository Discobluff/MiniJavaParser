#include <parser/prod_table.h>
#include <parser/parse.h>
#include <grammar/grammar_data/grammar.h>
#include "ast/nnast.h"

extern ASTStack *astk;
extern Stack *stk;
extern Production prod_table;



void parseFile(Lexer *lex) {
    File* file = create_file();
    parseClasse_list();
    while(astk->top > 0) {
        AST* class_ast = ast_stack_pop(astk);
        if (class_ast->kind == AST_CLASS) {
            file_add_class(file, class_ast->data.class_decl);
        } else {
            // Handle error: expected class AST
        }
    }
    ast_stack_push(astk, file);
}

void parseClass(ASTStack* astk) {

}

void parseClasse_list() {

}