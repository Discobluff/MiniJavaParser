#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type_checker.h"

// === Réimplémentation locale des fonctions static pour les tests ===

static Scope* scope_create(Scope* parent){
    Scope* new_scope = calloc(1, sizeof(Scope));
    new_scope->parent = parent;
    new_scope->vars = NULL;
    return new_scope;
}

static void declare_var(TypeContext* ctx, char* name, Type* type){
    VarSymbol* v = calloc(1, sizeof(VarSymbol));
    v->name = strdup(name);
    v->type = type;
    v->next = ctx->scope->vars;
    ctx->scope->vars = v;
}

static Type* lookup_var(TypeContext* ctx, const char* name){
    for(Scope* s = ctx->scope; s; s = s->parent){
        for(VarSymbol* v = s->vars; v; v = v->next){
            if(strcmp(v->name, name) == 0){
                return v->type;
            }
        }
    }
    return NULL;
}

static Scope* push_scope(TypeContext* ctx){
    Scope* s = calloc(1, sizeof(Scope));
    s->parent = ctx->scope;
    ctx->scope = s;
    return s;
}

static void scope_pop(TypeContext* ctx){
    if(ctx->scope){
        Scope* parent = ctx->scope->parent;
        VarSymbol* v = ctx->scope->vars;
        while(v){
            VarSymbol* next = v->next;
            free(v->name);
            free(v);
            v = next;
        }
        free(ctx->scope);
        ctx->scope = parent;
    }
}

// === Fonctions utilitaires pour les tests ===

static TypeContext* create_test_context(){
    TypeContext* ctx = calloc(1, sizeof(TypeContext));
    ctx->scope = scope_create(NULL);
    return ctx;
}

static void free_test_context(TypeContext* ctx){
    while(ctx->scope){
        scope_pop(ctx);
    }
    free(ctx);
}

// === Tests pour declare_var ===

void test_declare_var_single(){
    printf("Test: declare_var_single... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    
    declare_var(ctx, "x", int_type);
    
    assert(ctx->scope->vars != NULL);
    assert(strcmp(ctx->scope->vars->name, "x") == 0);
    assert(ctx->scope->vars->type->kind == TYPE_INT);
    assert(ctx->scope->vars->next == NULL);
    
    free_test_context(ctx);
    free(int_type);
    printf("OK\n");
}

void test_declare_var_multiple(){
    printf("Test: declare_var_multiple... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    Type* bool_type = create_type(TYPE_BOOLEAN);
    Type* string_type = create_type(TYPE_STRING);
    
    declare_var(ctx, "a", int_type);
    declare_var(ctx, "b", bool_type);
    declare_var(ctx, "c", string_type);
    
    // Les variables sont ajoutées en tête de liste (ordre inversé)
    assert(ctx->scope->vars != NULL);
    assert(strcmp(ctx->scope->vars->name, "c") == 0);
    assert(strcmp(ctx->scope->vars->next->name, "b") == 0);
    assert(strcmp(ctx->scope->vars->next->next->name, "a") == 0);
    
    free_test_context(ctx);
    free(int_type);
    free(bool_type);
    free(string_type);
    printf("OK\n");
}

void test_declare_var_name_copy(){
    printf("Test: declare_var_name_copy... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    
    char name[] = "variable";
    declare_var(ctx, name, int_type);
    
    // Modifier le nom original ne doit pas affecter la variable déclarée
    name[0] = 'X';
    
    assert(strcmp(ctx->scope->vars->name, "variable") == 0);
    
    free_test_context(ctx);
    free(int_type);
    printf("OK\n");
}

// === Tests pour lookup_var ===

void test_lookup_var_found(){
    printf("Test: lookup_var_found... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    
    declare_var(ctx, "x", int_type);
    
    Type* result = lookup_var(ctx, "x");
    assert(result != NULL);
    assert(result->kind == TYPE_INT);
    
    free_test_context(ctx);
    free(int_type);
    printf("OK\n");
}

void test_lookup_var_not_found(){
    printf("Test: lookup_var_not_found... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    
    declare_var(ctx, "x", int_type);
    
    Type* result = lookup_var(ctx, "y");
    assert(result == NULL);
    
    free_test_context(ctx);
    free(int_type);
    printf("OK\n");
}

void test_lookup_var_empty_scope(){
    printf("Test: lookup_var_empty_scope... ");
    
    TypeContext* ctx = create_test_context();
    
    Type* result = lookup_var(ctx, "x");
    assert(result == NULL);
    
    free_test_context(ctx);
    printf("OK\n");
}

void test_lookup_var_multiple_vars(){
    printf("Test: lookup_var_multiple_vars... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    Type* bool_type = create_type(TYPE_BOOLEAN);
    Type* string_type = create_type(TYPE_STRING);
    
    declare_var(ctx, "a", int_type);
    declare_var(ctx, "b", bool_type);
    declare_var(ctx, "c", string_type);
    
    Type* result_a = lookup_var(ctx, "a");
    Type* result_b = lookup_var(ctx, "b");
    Type* result_c = lookup_var(ctx, "c");
    
    assert(result_a != NULL && result_a->kind == TYPE_INT);
    assert(result_b != NULL && result_b->kind == TYPE_BOOLEAN);
    assert(result_c != NULL && result_c->kind == TYPE_STRING);
    
    free_test_context(ctx);
    free(int_type);
    free(bool_type);
    free(string_type);
    printf("OK\n");
}

// === Tests pour lookup_var avec scopes imbriqués ===

void test_lookup_var_parent_scope(){
    printf("Test: lookup_var_parent_scope... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    Type* bool_type = create_type(TYPE_BOOLEAN);
    
    declare_var(ctx, "x", int_type);  // Scope parent
    
    push_scope(ctx);  // Nouveau scope enfant
    declare_var(ctx, "y", bool_type);
    
    // Doit trouver x dans le scope parent
    Type* result_x = lookup_var(ctx, "x");
    Type* result_y = lookup_var(ctx, "y");
    
    assert(result_x != NULL && result_x->kind == TYPE_INT);
    assert(result_y != NULL && result_y->kind == TYPE_BOOLEAN);
    
    free_test_context(ctx);
    free(int_type);
    free(bool_type);
    printf("OK\n");
}

void test_lookup_var_shadowing(){
    printf("Test: lookup_var_shadowing... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    Type* bool_type = create_type(TYPE_BOOLEAN);
    
    declare_var(ctx, "x", int_type);  // x est int dans le scope parent
    
    push_scope(ctx);
    declare_var(ctx, "x", bool_type);  // x est boolean dans le scope enfant (shadowing)
    
    // Doit trouver x du scope courant (boolean), pas celui du parent
    Type* result = lookup_var(ctx, "x");
    assert(result != NULL && result->kind == TYPE_BOOLEAN);
    
    scope_pop(ctx);
    
    // Après avoir quitté le scope enfant, doit trouver x du parent (int)
    result = lookup_var(ctx, "x");
    assert(result != NULL && result->kind == TYPE_INT);
    
    free_test_context(ctx);
    free(int_type);
    free(bool_type);
    printf("OK\n");
}

void test_lookup_var_deeply_nested(){
    printf("Test: lookup_var_deeply_nested... ");
    
    TypeContext* ctx = create_test_context();
    Type* string_type = create_type(TYPE_STRING);
    
    declare_var(ctx, "root_var", string_type);
    
    // Créer 5 niveaux de scope imbriqués
    for(int i = 0; i < 5; i++){
        push_scope(ctx);
    }
    
    // Doit toujours trouver la variable du scope racine
    Type* result = lookup_var(ctx, "root_var");
    assert(result != NULL && result->kind == TYPE_STRING);
    
    free_test_context(ctx);
    free(string_type);
    printf("OK\n");
}

void test_lookup_var_out_of_scope(){
    printf("Test: lookup_var_out_of_scope... ");
    
    TypeContext* ctx = create_test_context();
    Type* int_type = create_type(TYPE_INT);
    
    push_scope(ctx);
    declare_var(ctx, "local", int_type);
    scope_pop(ctx);
    
    // La variable n'existe plus après scope_pop
    Type* result = lookup_var(ctx, "local");
    assert(result == NULL);
    
    free_test_context(ctx);
    free(int_type);
    printf("OK\n");
}

// === Main ===

int main(){
    printf("=== Tests unitaires pour declare_var et lookup_var ===\n\n");
    
    printf("--- Tests declare_var ---\n");
    test_declare_var_single();
    test_declare_var_multiple();
    test_declare_var_name_copy();
    
    printf("\n--- Tests lookup_var ---\n");
    test_lookup_var_found();
    test_lookup_var_not_found();
    test_lookup_var_empty_scope();
    test_lookup_var_multiple_vars();
    
    printf("\n--- Tests scopes imbriqués ---\n");
    test_lookup_var_parent_scope();
    test_lookup_var_shadowing();
    test_lookup_var_deeply_nested();
    test_lookup_var_out_of_scope();
    
    printf("\n=== Tous les tests sont passés ! ===\n");
    return 0;
}
