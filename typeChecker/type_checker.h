#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "ast/ast.h"

typedef enum{
    SYM_VAR,
    SYM_FIELD,
    SYM_METHOD,
    SYM_CLASS,
} SymbolKind;

typedef struct Symbol {
    char* name;
    SymbolKind kind;
    Type* type;
    Method* method;
    Field* field;
    ClassDecl* class_decl;
    struct Symbol* next;
}Symbol;

typedef struct VarSymbol{
    char* name;
    Type* type;
    struct VarSymbol* next;
}VarSymbol;

typedef struct Scope{
    struct Scope* parent;
    VarSymbol* vars;
}Scope;

typedef struct{
    File* file;
    Scope* scope;
    ClassDecl* current_class;
    Method* current_method;
}TypeContext;


void type_check_file(File* file);

int type_equals(Type* a, Type* b);
int type_compatible(File* file,Type* expected, Type* actual);

ClassDecl* find_class(File* file, const char* name);
Method* find_method(File* file, ClassDecl* cls, const char* name);
Field* find_field(File* file, ClassDecl* cls, const char* name);
int is_subclass(File* file, const char* child, const char* parent);

#endif