#include "prod_table.h"

void init_prod_table(void)
{
	// productions FIL
    prod_table[FIL][TOKEN_CLASS] = (Production) {.exp = {.items = {{.rule = CLASS_LIST, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[FIL][TOKEN_PUBLIC] = (Production) {.exp = {.items = {{.rule = CLASS_LIST, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[FIL][TOKEN_STATIC] = (Production) {.exp = {.items = {{.rule = CLASS_LIST, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[FIL][TOKEN_EOF] = (Production) {.exp = {.items = {{.rule = CLASS_LIST, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions CLASS_LIST
    prod_table[CLASS_LIST][TOKEN_CLASS] = (Production) {.exp = {.items = {{.rule = CLASSE, .type = RULE}, {.rule = CLASS_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[CLASS_LIST][TOKEN_PUBLIC] = (Production) {.exp = {.items = {{.rule = CLASSE, .type = RULE}, {.rule = CLASS_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[CLASS_LIST][TOKEN_STATIC] = (Production) {.exp = {.items = {{.rule = CLASSE, .type = RULE}, {.rule = CLASS_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[CLASS_LIST][TOKEN_EOF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// productions CLASSE
    prod_table[CLASSE][TOKEN_CLASS] = (Production) {.exp = {.items = {{.rule = PUBLIC_OPT, .type = RULE}, {.rule = STATIC_OPT, .type = RULE}, {.token = TOKEN_CLASS, .type = TOKEN}, {.token = TOKEN_ID_TYPE, .type = TOKEN}, {.rule = EXTENDS_OPT, .type = RULE}, {.token = TOKEN_LACC, .type = TOKEN}, {.rule = DECL_LIST, .type = RULE}, {.token = TOKEN_RACC, .type = TOKEN}}, .count = 8}, .action = action_stub};
    prod_table[CLASSE][TOKEN_PUBLIC] = (Production) {.exp = {.items = {{.rule = PUBLIC_OPT, .type = RULE}, {.rule = STATIC_OPT, .type = RULE}, {.token = TOKEN_CLASS, .type = TOKEN}, {.token = TOKEN_ID_TYPE, .type = TOKEN}, {.rule = EXTENDS_OPT, .type = RULE}, {.token = TOKEN_LACC, .type = TOKEN}, {.rule = DECL_LIST, .type = RULE}, {.token = TOKEN_RACC, .type = TOKEN}}, .count = 8}, .action = action_stub};
    prod_table[CLASSE][TOKEN_STATIC] = (Production) {.exp = {.items = {{.rule = PUBLIC_OPT, .type = RULE}, {.rule = STATIC_OPT, .type = RULE}, {.token = TOKEN_CLASS, .type = TOKEN}, {.token = TOKEN_ID_TYPE, .type = TOKEN}, {.rule = EXTENDS_OPT, .type = RULE}, {.token = TOKEN_LACC, .type = TOKEN}, {.rule = DECL_LIST, .type = RULE}, {.token = TOKEN_RACC, .type = TOKEN}}, .count = 8}, .action = action_stub};

	// productions PUBLIC_OPT
    prod_table[PUBLIC_OPT][TOKEN_CLASS] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[PUBLIC_OPT][TOKEN_PUBLIC] = (Production) {.exp = {.items = {{.token = TOKEN_PUBLIC, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[PUBLIC_OPT][TOKEN_STATIC] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// productions STATIC_OPT
    prod_table[STATIC_OPT][TOKEN_CLASS] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STATIC_OPT][TOKEN_STATIC] = (Production) {.exp = {.items = {{.token = TOKEN_STATIC, .type = TOKEN}}, .count = 1}, .action = action_stub};

	// productions EXTENDS_OPT
    prod_table[EXTENDS_OPT][TOKEN_LACC] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EXTENDS_OPT][TOKEN_EXTENDS] = (Production) {.exp = {.items = {{.token = TOKEN_EXTENDS, .type = TOKEN}, {.token = TOKEN_ID_TYPE, .type = TOKEN}}, .count = 2}, .action = action_stub};

	// productions DECL_LIST
    prod_table[DECL_LIST][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECL_LIST][TOKEN_RACC] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[DECL_LIST][TOKEN_PUBLIC] = (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECL_LIST][TOKEN_STATIC] = (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECL_LIST][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECL_LIST][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECL_LIST][TOKEN_INT] = (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECL_LIST][TOKEN_VOID] = (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions DECL_TYPE
    prod_table[DECL_TYPE][TOKEN_ID] = (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = DECL_TYPE_IDENT, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions DECL_TYPE_IDENT
    prod_table[DECL_TYPE_IDENT][TOKEN_PTSVG] = (Production) {.exp = {.items = {{.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[DECL_TYPE_IDENT][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = CONST_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions DECLA
    prod_table[DECLA][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = DECL_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[DECLA][TOKEN_PUBLIC] = (Production) {.exp = {.items = {{.token = TOKEN_PUBLIC, .type = TOKEN}, {.rule = DECL_PUBLIC, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECLA][TOKEN_STATIC] = (Production) {.exp = {.items = {{.token = TOKEN_STATIC, .type = TOKEN}, {.rule = CONST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECLA][TOKEN_ID] = (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = CONST_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECLA][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = DECL_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[DECLA][TOKEN_INT] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = DECL_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[DECLA][TOKEN_VOID] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = DECL_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions DECL_PUBLIC
    prod_table[DECL_PUBLIC][TOKEN_STATIC] = (Production) {.exp = {.items = {{.token = TOKEN_STATIC, .type = TOKEN}, {.rule = CONST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[DECL_PUBLIC][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = CONST, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions CONST_TAIL
    prod_table[CONST_TAIL][TOKEN_LPAR] = (Production) {.exp = {.items = {{.token = TOKEN_LPAR, .type = TOKEN}, {.rule = PARAMS_OPT, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.token = TOKEN_LACC, .type = TOKEN}, {.rule = STMT_LIST, .type = RULE}, {.token = TOKEN_RACC, .type = TOKEN}}, .count = 6}, .action = action_stub};

	// productions CONST
    prod_table[CONST][TOKEN_ID] = (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = CONST_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions PARAMS_OPT
    prod_table[PARAMS_OPT][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[PARAMS_OPT][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[PARAMS_OPT][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[PARAMS_OPT][TOKEN_INT] = (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[PARAMS_OPT][TOKEN_VOID] = (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions PARAMS
    prod_table[PARAMS][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[PARAMS][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[PARAMS][TOKEN_INT] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[PARAMS][TOKEN_VOID] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions PARAMS2
    prod_table[PARAMS2][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[PARAMS2][TOKEN_VG] = (Production) {.exp = {.items = {{.token = TOKEN_VG, .type = TOKEN}, {.rule = PARAMS, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions TYPE
    prod_table[TYPE][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.token = TOKEN_ID_TYPE, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[TYPE][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.token = TOKEN_BOOLEAN, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[TYPE][TOKEN_INT] = (Production) {.exp = {.items = {{.token = TOKEN_INT, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[TYPE][TOKEN_VOID] = (Production) {.exp = {.items = {{.token = TOKEN_VOID, .type = TOKEN}}, .count = 1}, .action = action_stub};

	// productions EXPR_OPT
    prod_table[EXPR_OPT][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR_OPT][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions EXPR
    prod_table[EXPR][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[EXPR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions ASSIGN
    prod_table[ASSIGN][TOKEN_ID] = (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = ASSIGN_ID, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions ASSIGN_ID
    prod_table[ASSIGN_ID][TOKEN_PTSVG] = (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN_ID][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN_ID][TOKEN_RPAR] = (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN_ID][TOKEN_VG] = (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[ASSIGN_ID][TOKEN_BINOP_EQ] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_EQ, .type = TOKEN}, {.rule = EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ASSIGN_ID][TOKEN_PTS] = (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions LOR_EXPR
    prod_table[LOR_EXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LOR_EXPR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions LOR_EXPR_SUITE
    prod_table[LOR_EXPR_SUITE][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LOR_EXPR_SUITE][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LOR_EXPR_SUITE][TOKEN_VG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LOR_EXPR_SUITE][TOKEN_BINOP_OR] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_OR, .type = TOKEN}, {.rule = LAND_EXPR, .type = RULE}, {.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions LAND_EXPR
    prod_table[LAND_EXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LAND_EXPR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions LAND_EXPR_SUITE
    prod_table[LAND_EXPR_SUITE][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LAND_EXPR_SUITE][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LAND_EXPR_SUITE][TOKEN_VG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LAND_EXPR_SUITE][TOKEN_BINOP_OR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LAND_EXPR_SUITE][TOKEN_BINOP_AND] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_AND, .type = TOKEN}, {.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions EQ_EXPR
    prod_table[EQ_EXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[EQ_EXPR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions EQ_EXPR_SUITE
    prod_table[EQ_EXPR_SUITE][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EQ_EXPR_SUITE][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EQ_EXPR_SUITE][TOKEN_VG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_OR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_AND] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_2EQUAL] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_2EQUAL, .type = TOKEN}, {.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_DIFF] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_DIFF, .type = TOKEN}, {.rule = RELATIONAL_EXPR, .type = RULE}, {.rule = EQ_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions RELATIONAL_EXPR
    prod_table[RELATIONAL_EXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[RELATIONAL_EXPR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions RELATIONAL_EXPR_SUITE
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_VG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_OR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_AND] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_2EQUAL] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_DIFF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_INF] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_INF, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_INFEG] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_INFEG, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_SUP] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUP, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_SUPEG] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUPEG, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[RELATIONAL_EXPR_SUITE][TOKEN_INSTANCEOF] = (Production) {.exp = {.items = {{.token = TOKEN_INSTANCEOF, .type = TOKEN}, {.rule = TYPE, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions ADD
    prod_table[ADD][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[ADD][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions ADD_SUITE
    prod_table[ADD_SUITE][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_VG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_OR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_AND] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_2EQUAL] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_DIFF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_INF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_INFEG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_SUP] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_SUPEG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_INSTANCEOF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_ADD] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_ADD, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[ADD_SUITE][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUB, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions MULT
    prod_table[MULT][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[MULT][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions MULT_SUITE
    prod_table[MULT_SUITE][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_VG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_OR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_AND] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_2EQUAL] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_DIFF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_INF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_INFEG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_SUP] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_SUPEG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_INSTANCEOF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_ADD] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_MULT] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MULT, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_DIV] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_DIV, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[MULT_SUITE][TOKEN_BINOP_MOD] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MOD, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions UNARY
    prod_table[UNARY][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUB, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[UNARY][TOKEN_OPP] = (Production) {.exp = {.items = {{.token = TOKEN_OPP, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[UNARY][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[UNARY][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions C_O_P
    prod_table[C_O_P][TOKEN_LPAR] = (Production) {.exp = {.items = {{.token = TOKEN_LPAR, .type = TOKEN}, {.rule = C_O_P_PAR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[C_O_P][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions C_O_P_PAR
    prod_table[C_O_P_PAR][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = C_O_P, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = C_O_P, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_INT] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = C_O_P, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_VOID] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = C_O_P, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[C_O_P_PAR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions POSTFIX_EXPR
    prod_table[POSTFIX_EXPR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[POSTFIX_EXPR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions POSTFIX_TAIL
    prod_table[POSTFIX_TAIL][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_LPAR] = (Production) {.exp = {.items = {{.token = TOKEN_LPAR, .type = TOKEN}, {.rule = LEXPR_OPT, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 4}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_VG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_OR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_AND] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_2EQUAL] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_DIFF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_INF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_INFEG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_SUP] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_SUPEG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_INSTANCEOF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_ADD] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_MULT] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_DIV] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_BINOP_MOD] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[POSTFIX_TAIL][TOKEN_PTS] = (Production) {.exp = {.items = {{.token = TOKEN_PTS, .type = TOKEN}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 3}, .action = action_stub};

	// productions PRIMARY
    prod_table[PRIMARY][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.token = TOKEN_NUMBER, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[PRIMARY][TOKEN_STR] = (Production) {.exp = {.items = {{.token = TOKEN_STR, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[PRIMARY][TOKEN_TRUE] = (Production) {.exp = {.items = {{.token = TOKEN_TRUE, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[PRIMARY][TOKEN_FALSE] = (Production) {.exp = {.items = {{.token = TOKEN_FALSE, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[PRIMARY][TOKEN_THIS] = (Production) {.exp = {.items = {{.token = TOKEN_THIS, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[PRIMARY][TOKEN_NULL] = (Production) {.exp = {.items = {{.token = TOKEN_NULL, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[PRIMARY][TOKEN_NEW] = (Production) {.exp = {.items = {{.token = TOKEN_NEW, .type = TOKEN}, {.token = TOKEN_ID, .type = TOKEN}, {.token = TOKEN_LPAR, .type = TOKEN}, {.rule = LEXPR_OPT, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 5}, .action = action_stub};

	// productions PRIMARY_PAR
    prod_table[PRIMARY_PAR][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[PRIMARY_PAR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};

	// productions LEXPR_OPT
    prod_table[LEXPR_OPT][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[LEXPR_OPT][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};

	// productions LEXPR
    prod_table[LEXPR][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[LEXPR][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions LEXPR_EXPR
    prod_table[LEXPR_EXPR][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[LEXPR_EXPR][TOKEN_VG] = (Production) {.exp = {.items = {{.token = TOKEN_VG, .type = TOKEN}, {.rule = LEXPR, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions STMT_LIST
    prod_table[STMT_LIST][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_LACC] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_RACC] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_PTSVG] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_INT] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_VOID] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_IF] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_FOR] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_LIST][TOKEN_RETURN] = (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};

	// productions STMT
    prod_table[STMT][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[STMT][TOKEN_LACC] = (Production) {.exp = {.items = {{.rule = BLOCK, .type = RULE}}, .count = 1}, .action = action_stub};
    prod_table[STMT][TOKEN_ID] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_PTSVG] = (Production) {.exp = {.items = {{.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[STMT][TOKEN_LPAR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[STMT][TOKEN_INT] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[STMT][TOKEN_VOID] = (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
    prod_table[STMT][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_OPP] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_NUMBER] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_STR] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_TRUE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_FALSE] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_THIS] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_NULL] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_NEW] = (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
    prod_table[STMT][TOKEN_IF] = (Production) {.exp = {.items = {{.token = TOKEN_IF, .type = TOKEN}, {.token = TOKEN_LPAR, .type = TOKEN}, {.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = BLOCK, .type = RULE}, {.rule = STMT_IF, .type = RULE}}, .count = 6}, .action = action_stub};
    prod_table[STMT][TOKEN_FOR] = (Production) {.exp = {.items = {{.token = TOKEN_FOR, .type = TOKEN}, {.token = TOKEN_LPAR, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = STMT, .type = RULE}}, .count = 9}, .action = action_stub};
    prod_table[STMT][TOKEN_RETURN] = (Production) {.exp = {.items = {{.token = TOKEN_RETURN, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 3}, .action = action_stub};

	// productions STMT_TYPE_IDENT
    prod_table[STMT_TYPE_IDENT][TOKEN_PTSVG] = (Production) {.exp = {.items = {{.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 1}, .action = action_stub};
    prod_table[STMT_TYPE_IDENT][TOKEN_BINOP_EQ] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_EQ, .type = TOKEN}, {.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 3}, .action = action_stub};

	// productions STMT_IF
    prod_table[STMT_IF][TOKEN_ID_TYPE] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_LACC] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_RACC] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_ID] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_PTSVG] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_LPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_INT] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_VOID] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_OPP] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_NUMBER] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_STR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_TRUE] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_FALSE] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_THIS] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_NULL] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_NEW] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_IF] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_ELSE] = (Production) {.exp = {.items = {{.token = TOKEN_ELSE, .type = TOKEN}, {.rule = BLOCK, .type = RULE}}, .count = 2}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_FOR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
    prod_table[STMT_IF][TOKEN_RETURN] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// productions BLOCK
    prod_table[BLOCK][TOKEN_LACC] = (Production) {.exp = {.items = {{.token = TOKEN_LACC, .type = TOKEN}, {.rule = STMT_LIST, .type = RULE}, {.token = TOKEN_RACC, .type = TOKEN}}, .count = 3}, .action = action_stub};
}
