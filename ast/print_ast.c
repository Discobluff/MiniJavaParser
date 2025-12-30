#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast/ast.h>
#include "print_ast.h"

static void print_indent(int indent){
    for (int i = 0; i < indent; i++) putchar(' ');
}

static const char *binop_to_str(BinaryOp op){
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_MOD: return "%";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_LT: return "<";
        case OP_LTE: return "<=";
        case OP_GT: return ">";
        case OP_GTE: return ">=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        case OP_ASSIGN: return "=";
        default: return "?";
    }
}

static const char *unaryop_to_str(UnaryOp op){
    switch (op){
        case UNARY_NOT: return "!";
        case UNARY_NEG: return "-";
        default: return "?";
    }
}

static void print_type(Type *t){
    if (!t) { printf("<null-type>"); return; }
    switch (t->kind){
        case TYPE_INT: printf("int"); break;
        case TYPE_BOOLEAN: printf("boolean"); break;
        case TYPE_STRING: printf("string"); break;
        case TYPE_VOID: printf("void"); break;
        case TYPE_CLASS: printf("class %s", t->class_name ? t->class_name : "<anon>"); break;
        default: printf("<unknown-type>"); break;
    }
}

static void print_expression(Expression *e, int indent);
static void print_statement(Statement *s, int indent);

static void print_literal(Literal *lit){
    switch (lit->kind){
        case LIT_INT: printf("%d", lit->int_val); break;
        case LIT_BOOLEAN: printf(lit->bool_val ? "true" : "false"); break;
        case LIT_STRING: printf("\"%s\"", lit->string_val ? lit->string_val : ""); break;
        case LIT_NULL: printf("null"); break;
        default: printf("<lit?>"); break;
    }
}

static void print_call(CallExpr *call, int indent){
    if (!call) { printf("<null-call>"); return; }
    printf("call\n");
    print_indent(indent+2); printf("callee: ");
    print_expression(call->callee, indent+2); printf("\n");
    print_indent(indent+2); printf("args:\n");
    for (int i = 0; i < call->arg_count; i++){
        print_indent(indent+4); printf("- ");
        print_expression(call->args[i], indent+4);
        printf("\n");
    }
}

static void print_new(NewExpr *n, int indent){
    if (!n) { printf("<null-new>"); return; }
    printf("new ");
    print_type(n->type);
    printf("\n");
    print_indent(indent+2); printf("args:\n");
    for (int i = 0; i < n->arg_count; i++){
        print_indent(indent+4); printf("- ");
        print_expression(n->args[i], indent+4);
        printf("\n");
    }
}

static void print_expression(Expression *e, int indent){
    if (!e) { printf("<null-expr>"); return; }
    switch (e->kind){
        case EXPR_LITERAL:
            print_literal(&e->literal);
            break;
        case EXPR_VARIABLE:
            printf("%s", e->var_name ? e->var_name : "<anon-var>");
            break;
        case EXPR_BINARY_OP:
            printf("(%s)\n", binop_to_str(e->binary.op));
            print_indent(indent+2); printf("left: "); print_expression(e->binary.left, indent+2); printf("\n");
            print_indent(indent+2); printf("right: "); print_expression(e->binary.right, indent+2);
            break;
        case EXPR_UNARY_OP:
            printf("(%s)\n", unaryop_to_str(e->unary.op));
            print_indent(indent+2); print_expression(e->unary.operand, indent+2);
            break;
        case EXPR_CALL:
            print_call(&e->call, indent);
            break;
        case EXPR_INSTOF:
            printf("instanceof\n");
            print_indent(indent+2); printf("expr: "); print_expression(e->instof.val, indent+2); printf("\n");
            print_indent(indent+2); printf("type: "); print_type(e->instof.comp);
            break;
        case EXPR_NEW:
            print_new(&e->new_expr, indent);
            break;
        case EXPR_FIELD_ACCESS:
            printf("field_access\n");
            print_indent(indent+2); printf("object: "); print_expression(e->field_access.object, indent+2); printf("\n");
            print_indent(indent+2); printf("field: %s", e->field_access.field_name ? e->field_access.field_name : "");
            break;
        case EXPR_CAST:
            printf("cast\n");
            print_indent(indent+2); printf("type: "); print_type(e->cast.target_type); printf("\n");
            print_indent(indent+2); printf("expr: "); print_expression(e->cast.expr, indent+2);
            break;
        default:
            printf("<expr?>");
            break;
    }
}

static void print_parameter(Parameter *p, int indent){
    if (!p) { printf("<null-param>"); return; }
    print_type(p->type); printf(" %s", p->name ? p->name : "");
}

static void print_field(Field *f, int indent){
    if (!f) { printf("<null-field>\n"); return; }
    print_indent(indent); printf("Field: ");
    print_type(f->type); printf(" %s", f->name ? f->name : "");
    if (f->initializer){ printf(" = "); print_expression(f->initializer, indent); }
    printf("\n");
}

static void print_method(Method *m, int indent){
    if (!m) { printf("<null-method>\n"); return; }
    print_indent(indent); printf("Method: %s (", m->name ? m->name : "<anon>");
    print_type(m->return_type); printf(")\n");
    if (m->param_count > 0){
        print_indent(indent+2); printf("params:\n");
        for (int i = 0; i < m->param_count; i++){
            print_indent(indent+4); print_parameter(m->params[i], indent+4); printf("\n");
        }
    }
    print_indent(indent+2); printf("body:\n");
    print_statement(m->body, indent+4);
    printf("\n");
}

static void print_constructor(Constructor *c, int indent){
    if (!c) { printf("<null-ctor>\n"); return; }
    print_indent(indent); printf("Constructor:\n");
    if (c->param_count > 0){
        print_indent(indent+2); printf("params:\n");
        for (int i = 0; i < c->param_count; i++){
            print_indent(indent+4); print_parameter(c->params[i], indent+4); printf("\n");
        }
    }
    print_indent(indent+2); printf("body:\n");
    print_statement(c->body, indent+4);
    printf("\n");
}

static void print_class(ClassDecl *cls, int indent){
    if (!cls) { printf("<null-class>\n"); return; }
    print_indent(indent); printf("Class: %s", cls->name ? cls->name : "<anon>");
    if (cls->super_class) printf(" extends %s", cls->super_class);
    printf("\n");
    for (int i = 0; i < cls->field_count; i++){
        print_field(cls->fields[i], indent+2);
    }
    for (int i = 0; i < cls->constructor_count; i++){
        print_constructor(cls->constructors[i], indent+2);
    }
    for (int i = 0; i < cls->method_count; i++){
        print_method(cls->methods[i], indent+2);
    }
}

static void print_block(Block *b, int indent){
    if (!b) { print_indent(indent); printf("<null-block>\n"); return; }
    print_indent(indent); printf("Block:\n");
    for (int i = 0; i < b->count; i++){
        print_statement(b->stmts[i], indent+2);
        printf("\n");
    }
}

static void print_statement(Statement *s, int indent){
    if (!s) { print_indent(indent); printf("<null-stmt>\n"); return; }
    switch (s->kind){
        case STMT_EXPR:
            print_indent(indent); printf("ExprStmt: "); print_expression(s->expr, indent); printf("\n");
            break;
        case STMT_RETURN:
            print_indent(indent); printf("Return");
            if (s->return_val) { printf(": "); print_expression(s->return_val, indent); }
            printf("\n");
            break;
        case STMT_IF:
            print_indent(indent); printf("If:\n");
            print_indent(indent+2); printf("cond: "); print_expression(s->if_stmt.condition, indent+2); printf("\n");
            print_indent(indent+2); printf("then:\n"); print_statement(s->if_stmt.then_branch, indent+4); printf("\n");
            if (s->if_stmt.else_branch){ print_indent(indent+2); printf("else:\n"); print_statement(s->if_stmt.else_branch, indent+4); printf("\n"); }
            break;
        case STMT_FOR:
            print_indent(indent); printf("For:\n");
            if (s->for_stmt.init){ print_indent(indent+2); printf("init:\n"); print_statement(s->for_stmt.init, indent+4); }
            if (s->for_stmt.condition){ print_indent(indent+2); printf("cond: "); print_expression(s->for_stmt.condition, indent+2); printf("\n"); }
            if (s->for_stmt.increment){ print_indent(indent+2); printf("incr: "); print_expression(s->for_stmt.increment, indent+2); printf("\n"); }
            print_indent(indent+2); printf("body:\n"); print_statement(s->for_stmt.body, indent+4); printf("\n");
            break;
        case STMT_VAR_DECL:
            print_indent(indent); printf("VarDecl: "); print_type(s->var_decl.type); printf(" %s", s->var_decl.name ? s->var_decl.name : "");
            if (s->var_decl.initializer){ printf(" = "); print_expression(s->var_decl.initializer, indent); }
            printf("\n");
            break;
        case STMT_BREAK:
            print_indent(indent); printf("break\n"); break;
        case STMT_CONTINUE:
            print_indent(indent); printf("continue\n"); break;
        case STMT_BLOCK:
            print_block(&s->block, indent);
            break;
        default:
            print_indent(indent); printf("<stmt?>\n"); break;
    }
}

void print_file_ast(File *file){
    if (!file){ printf("<null-file>\n"); return; }
    printf("File\n");
    for (int i = 0; i < file->class_count; i++){
        print_class(file->classes[i], 2);
        printf("\n");
    }
}
