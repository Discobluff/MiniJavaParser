#include "type_checker.h"
#include <stdio.h>
#include <stdlib.h>

static Type* type_error(){
    static Type* err = NULL;
    if (!err) {
        err = malloc(sizeof(Type));
        err->kind = TYPE_ERROR;
        err->class_name = NULL;
    }
    return err;
}


static void type_error_msg(const char* msg){
    fprintf(stderr, "[TYPE_ERROR] %s\n", msg);
}


static Scope* push_scope(TypeContext* ctx){
    Scope* s=calloc(1,sizeof(Scope));
    s->parent = ctx->scope;
    ctx->scope=s;
    return s;
}

Scope* scope_create(Scope* parent){
    Scope* new_scope = calloc(1, sizeof(Scope));
    new_scope->parent = parent;
    new_scope->vars = NULL;
    return new_scope;
}

static void scope_pop(Scope** scope){
    if(*scope){
        Scope* parent_scope = (*scope)->parent;
        free(*scope);
        *scope = parent_scope;
    }
}

static void declare_var(TypeContext* ctx,char* name, Type* type){
    VarSymbol* v=calloc(1,sizeof(VarSymbol));
    v->name=strdup(name);
    v->type=type;
    v->next=ctx->scope->vars;
    ctx->scope->vars=v;
}


static Type* lookup_var(TypeContext* ctx, const char* name){
    for(Scope* s=ctx->scope; s; s=s->parent){
        for(VarSymbol* v = s->vars; v; v= v-> next){
            if(strcmp(v->name, name)==0){
                return v->type;
            }
        }
    }

    Field* f = find_field(ctx->current_class, name);
    if (f) {
        return f->type;
    }
    return NULL;
}


int type_equals(Type* a, Type* b){
    if(!a|| !b){
        return 0;
    }
    if(a->kind != b->kind){
        return 0;
    }
    if(a->kind == TYPE_CLASS){
        return strcmp(a->class_name, b->class_name)==0;
    }
    return 1;
}

int is_subclass(File* file, const char* child, const char* parent){
    while(child){
        if(strcmp(child,parent)==0){
            return 1;
        }
        ClassDecl* cls = find_class(file,child);
        if(!cls){
            break;
        }
        child=cls->super_class; 
    }
    return 0;
}

int type_compatible(File* file,Type* expected, Type* actual){
    if(!expected || !actual){
        return 0;
    }
    if(actual->kind == TYPE_ERROR){
        return 1;
    }
    if(type_equals(expected,actual)){
        return 1;
    }
    if(expected->kind == TYPE_CLASS && actual->kind == TYPE_CLASS){
        return is_subclass(file, actual->class_name, expected->class_name);
    }
    if (expected->kind == TYPE_CLASS && actual->kind == TYPE_NULL){
        return 1;
    }
    return 0;
}

ClassDecl* find_class(File* file, const char* name){
    for(int i=0; i<file->class_count; i++){
        if(strcmp(file->classes[i]->name,name)==0){
            return file->classes[i];
        }
    }
    return NULL;
}

Method* find_method(ClassDecl* cls, const char* name){
    for(int i=0; i<cls->method_count; i++){
        if(strcmp(cls->methods[i]->name,name)==0){
            return cls->methods[i];
        }
    }
    if(cls->super_class){
        ClassDecl* sup = find_class(NULL, cls->super_class);
        if(sup){
            return find_method(sup,name);
        }
    }
    return NULL;
}

Field* find_field(ClassDecl* cls, const char* name){
    for(int i=0; i<cls->field_count; i++){
        if(strcmp(cls->fields[i]->name,name)==0){
            return cls->fields[i];
        }
    }
    if(cls->super_class){
        ClassDecl* sup=find_class(NULL, cls->super_class);
        if(sup){
            return find_field(sup,name);
        }
    }
    return NULL;
}

static Type* type_check_expression(TypeContext* ctx, Expression* exp);
static void type_check_statement(TypeContext* ctx, Statement* stmt);

static Type* check_binary(TypeContext* ctx, Expression* exp){
    if (exp->binary.left->kind == EXPR_LITERAL) {
        type_error_msg("impossible d'assigner à un littéral");
        return exp->type = type_error();
    }
    Type* l= type_check_expression(ctx, exp->binary.left);
    Type* r= type_check_expression(ctx, exp->binary.right);

    switch(exp->binary.op){
        case OP_ADD: 
            if(l-> kind == TYPE_INT && r->kind == TYPE_INT){
                return exp->type = create_type(TYPE_INT);
            }

            if (l->kind == TYPE_STRING && r->kind == TYPE_STRING) {
                return exp->type = create_type(TYPE_STRING);

            }
            type_error_msg("les opérateurs arithmétiques nécessitent des types int");
            return exp->type=type_error();
            
        case OP_SUB:
        case OP_MUL: case OP_DIV : case OP_MOD:
            if(l-> kind != TYPE_INT || r->kind != TYPE_INT){
                        type_error_msg("les opérateurs arithmétiques nécessitent des types int");
                        return exp->type=type_error();
                    }
            return exp->type = create_type(TYPE_INT);
        case OP_AND: case OP_OR:
            if(l->kind != TYPE_BOOLEAN || r->kind != TYPE_BOOLEAN){
                        type_error_msg("les opérations logiques nécessitent des booléens");
                        return exp->type=type_error();
                    }
            return exp->type = create_type(TYPE_BOOLEAN);
        case OP_EQ: case OP_NEQ:
            if(!type_compatible(ctx->file,l,r)){
                    type_error_msg("types incompatibles pour une comparaison");
                    return exp->type=type_error();
                }
            return exp->type = create_type(TYPE_BOOLEAN);
        case OP_LT: case OP_LTE:
        case OP_GT: case OP_GTE:
            if(l->kind != TYPE_INT || r->kind != TYPE_INT){
                        type_error_msg("les comparaisons se font sur des int");
                        return exp->type = type_error();
                    }
            return exp->type = create_type(TYPE_BOOLEAN);
        case OP_ASSIGN:
            if(!type_compatible(ctx->file,l,r)){
                        type_error_msg("types incompatibles pour une assignation");
                        return exp->type=type_error();
                    }
            return exp->type = copy_type(l);
        default:
            return exp->type=type_error();
                    
    }
}

static Type* type_check_expression(TypeContext* ctx, Expression* expr){
    if(!expr){
        return create_type(TYPE_VOID);
    }
    switch(expr->kind){
        case EXPR_LITERAL:
        switch (expr->literal.kind){
            case LIT_INT:
                return expr->type = create_type(TYPE_INT);
            case LIT_BOOLEAN:
                return expr->type = create_type(TYPE_BOOLEAN);
            case LIT_STRING:
                return expr->type = create_type(TYPE_STRING);
            case LIT_NULL:
                return expr->type = create_type(TYPE_NULL);
            default:
                return expr->type=type_error();
        }
        case EXPR_VARIABLE:{
            Type* t=lookup_var(ctx, expr->var_name);
            if(!t){
                type_error_msg("utilisation d'une variable non déclarée");
                return expr->type = type_error();
            }
            return expr->type = copy_type(t);
        }
        case EXPR_BINARY_OP:{
            return check_binary(ctx,expr);
        }
        case EXPR_UNARY_OP:{
            Type* t = type_check_expression(ctx, expr -> unary.operand);
            if(expr->unary.op == UNARY_NOT && t->kind == TYPE_BOOLEAN){
                return expr->type=create_type(TYPE_BOOLEAN);
            }
            if(expr->unary.op == UNARY_NEG && t->kind == TYPE_INT){
                return expr->type = create_type(TYPE_INT);
            }
            type_error_msg("Opérateur unaire invalide");
            return expr->type = type_error();
        }
        case EXPR_CALL:{
            // L'appelant peut être: obj.method ou method (this implicite)
            Expression* callee = expr->call.callee;
            Method* method = NULL;
            ClassDecl* cls = NULL;
            
            if (callee->kind == EXPR_FIELD_ACCESS) {
                // Cas: obj.method(args)
                Type* obj_type = type_check_expression(ctx, callee->field_access.object);
                if (obj_type->kind != TYPE_CLASS) {
                    type_error_msg("appel de méthode sur un non-objet");
                    return expr->type = type_error();
                }
                cls = find_class(ctx->file, obj_type->class_name);
                if (!cls) {
                    type_error_msg("classe non trouvée");
                    return expr->type = type_error();
                }
                method = find_method(cls, callee->field_access.field_name);
                if (!method) {
                    type_error_msg("méthode non trouvée");
                    return expr->type = type_error();
                }
            } else if (callee->kind == EXPR_VARIABLE) {
                // Cas: method(args) - appel sur this implicite
                cls = ctx->current_class;
                method = find_method(cls, callee->var_name);
                if (!method) {
                    type_error_msg("méthode non trouvée dans la classe courante");
                    return expr->type = type_error();
                }
            } else {
                type_error_msg("forme d'appel de méthode non supportée");
                return expr->type = type_error();
            }
            
            // Vérifier le nombre d'arguments
            if (expr->call.arg_count != method->param_count) {
                type_error_msg("nombre d'arguments incorrect");
                return expr->type = type_error();
            }
            
            // Vérifier les types des arguments
            for (int i = 0; i < expr->call.arg_count; i++) {
                Type* arg_type = type_check_expression(ctx, expr->call.args[i]);
                if (!type_compatible(ctx->file, method->params[i]->type, arg_type)) {
                    type_error_msg("type d'argument incompatible");
                    return expr->type = type_error();
                }
            }
            
            // Retourner le type de retour de la méthode
            return expr->type = copy_type(method->return_type);
        }
        case EXPR_NEW:
            if(expr->new_expr.type->kind != TYPE_CLASS){
                type_error("new nécessite un type class");
            }
            return expr->type= copy_type(expr->new_expr.type);

        case EXPR_INSTOF: {
            Type* t=type_check_expression(ctx, expr->instof.val);
            if(t->kind != TYPE_CLASS){
                type_error_msg("instanceof sur un non_objet");
                return expr->type=type_error();
            }
            return expr->type = create_type(TYPE_BOOLEAN);
        }
        case EXPR_FIELD_ACCESS:{
            Type* obj =type_check_expression(ctx, expr->field_access.object);
            if(obj->kind != TYPE_CLASS){
                type_error_msg("accès d'un champ sur un non_objet");
                return expr->type = type_error();
            }
            ClassDecl* cls = find_class(ctx->file, obj->class_name);
            Field* f=find_field(cls, expr->field_access.field_name);
            if(!f){
                type_error_msg("champ inconnu");
                return expr->type=type_error();
            }
            return expr->type = copy_type(f->type);
        }
        case EXPR_CAST:
            type_check_expression(ctx, expr-> cast.expr);
            return expr->type = copy_type(expr->cast.target_type);

        default:
            return expr->type = type_error();

    }
    return NULL;
}

// static void type_check_block(TypeContext* ctx, Block* block){
//     ctx->scope = scope_create(ctx->scope);
//     for(int i=0; i<block->count; i++){
//         type_check_statement(ctx, block->stmts[i]);
//     }
//     scope_pop(&ctx->scope);
// }

static void type_check_statement(TypeContext* ctx, Statement* stmt){
    if(!stmt){
        return;
    }
    switch(stmt->kind){
        case STMT_EXPR:
            type_check_expression(ctx, stmt->expr);
            break;

        case STMT_RETURN:{
            Type* t = stmt->return_val ? type_check_expression(ctx, stmt->return_val) : create_type(TYPE_VOID);
            if(! type_compatible(ctx->file, ctx->current_method->return_type,t)){
                type_error_msg("type de retour ne correspond pas");
            }
            break;
        }
        case STMT_VAR_DECL:{
            if(stmt->var_decl.initializer){
                Type* t=type_check_expression(ctx, stmt->var_decl.initializer);
                if(!type_compatible(ctx->file,stmt->var_decl.type,t)){
                    type_error_msg("type d'initialisation non valide");
                }
            }
            declare_var(ctx, stmt->var_decl.name, stmt->var_decl.type);
            break;
        }
        case STMT_IF:
            if(type_check_expression(ctx, stmt->if_stmt.condition)->kind != TYPE_BOOLEAN){
                type_error_msg("La condition du if doit être un booléen");
            }
            type_check_statement(ctx, stmt->if_stmt.then_branch);
            type_check_statement(ctx, stmt->if_stmt.else_branch);
            break;

        case STMT_FOR:
            push_scope(ctx);
            type_check_statement(ctx, stmt->for_stmt.init);
            if(stmt->for_stmt.condition && type_check_expression(ctx, stmt->for_stmt.condition)->kind != TYPE_BOOLEAN){
                type_error_msg("la condition du for doit être un booléen");
            }
            type_check_expression(ctx,stmt->for_stmt.increment);
            type_check_statement(ctx, stmt->for_stmt.body);
            scope_pop(&ctx->scope);
            break;

        case STMT_BLOCK:
            push_scope(ctx);
            for(int i=0; i<stmt->block.count; i++){
                type_check_statement(ctx, stmt->block.stmts[i]);
            }
            scope_pop(&ctx->scope);
            break;

        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
}

static void type_check_method(TypeContext* ctx, Method* m){
    ctx->current_method=m;
    ctx->scope = scope_create(ctx->scope);

    for(int i=0; i< m->param_count; i++){
        declare_var(ctx,m->params[i]->name, m->params[i]->type);
    }
    type_check_statement(ctx, m->body);
    scope_pop(&ctx->scope);
}

static void type_check_class(TypeContext* ctx, ClassDecl* cls){
    ctx->current_class=cls;

    for(int i=0; i<cls->method_count; i++){
        type_check_method(ctx, cls->methods[i]);
    }
}

void type_check_file(File* file){
    TypeContext ctx = {0};
    ctx.file = file;
    for(int i=0; i< file->class_count; i++){
        type_check_class(&ctx, file->classes[i]);
    }
}