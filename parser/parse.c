/**
 * electric boogaloo
 * https://www.youtube.com/watch?v=Zo8v7FrM3mI
 * https://www.youtube.com/watch?v=B7nKzCRL_oo
 */

#include "parse.h"


ASTStack* astk;
Stack* stk;

static Production prod_table[RULE_SIZE][TOKENTYPE_SIZE];

// table of Productions indexed by non-terminals (Rule) and tokens (TokenType) santa maria madre de dios
static void init_prod_table(void)
{

	// FILE productions
	prod_table[FIL][TOKEN_CLASS] = (Production) {.exp = {.items = {{.rule = CLASS_LIST, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[FIL][TOKEN_PUBLIC]= (Production) {.exp = {.items = {{.rule = CLASS_LIST, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[FIL][TOKEN_EOF]   = (Production) {.exp = {.items = {{.rule = CLASS_LIST, .type = RULE}}, .count = 1}, .action = action_stub};

	// CLASS_LIST productions
	prod_table[CLASS_LIST][TOKEN_PUBLIC]= (Production) {.exp = {.items = {{.rule = CLASSE, .type = RULE}, {.rule = CLASS_LIST, .type =  RULE}}, .count = 2}, .action = action_stub};
	prod_table[CLASS_LIST][TOKEN_CLASS] = (Production) {.exp = {.items = {{.rule = CLASSE, .type = RULE}, {.rule = CLASS_LIST, .type =  RULE}}, .count = 2}, .action = action_stub};
	prod_table[CLASS_LIST][TOKEN_EOF]   = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// PUBLIC_OPT productions
	prod_table[PUBLIC_OPT][TOKEN_PUBLIC]= (Production) {.exp = {.items = {{.token = TOKEN_PUBLIC, .type = TOKEN}}, .count = 1}, .action = action_stub};
	prod_table[PUBLIC_OPT][TOKEN_CLASS]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[PUBLIC_OPT][TOKEN_LPAR]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// CLASS productions
	prod_table[CLASSE][TOKEN_CLASS]    = (Production) {.exp = {.items = {{.rule = PUBLIC_OPT, .type = RULE}, {.token = TOKEN_CLASS, .type = TOKEN}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = EXTENDS_OPT, .type = RULE}, {.token = TOKEN_LACC, .type = TOKEN}, {.rule = DECL_LIST, .type = RULE},{.token = TOKEN_RACC, .type = TOKEN}}, .count = 7}, .action = action_stub};
	prod_table[CLASSE][TOKEN_PUBLIC]   = (Production) {.exp = {.items = {{.rule = PUBLIC_OPT, .type = RULE}, {.token = TOKEN_CLASS, .type = TOKEN}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = EXTENDS_OPT, .type = RULE}, {.token = TOKEN_LACC, .type = TOKEN}, {.rule = DECL_LIST, .type = RULE},{.token = TOKEN_RACC, .type = TOKEN}}, .count = 7}, .action = action_stub};

	// EXTENDS_OPT productions
	prod_table[EXTENDS_OPT][TOKEN_LACC] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EXTENDS_OPT][TOKEN_EXTENDS] 	= (Production) {.exp = {.items = {{.token = TOKEN_EXTENDS, .type = TOKEN}, {.token = TOKEN_ID, .type = TOKEN}}, .count = 2 }, .action = action_stub};

	//DECL_LIST productions
	prod_table[DECL_LIST][TOKEN_PUBLIC]	= (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[DECL_LIST][TOKEN_ID]  	= (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[DECL_LIST][TOKEN_LPAR] 	= (Production) {.exp = {.items = {{.rule = DECLA, .type = RULE}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[DECL_LIST][TOKEN_RACC] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	//DECLA productions
	prod_table[DECLA][TOKEN_ID] 	= (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = DECL_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[DECLA][TOKEN_PUBLIC] = (Production) {.exp = {.items = {{.rule = PUBLIC_OPT, .type = RULE}, {.rule = METHODE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[DECLA][TOKEN_LPAR]	= (Production) {.exp = {.items = {{.rule = PUBLIC_OPT, .type = RULE}, {.rule = METHODE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[DECLA][TOKEN_LPAR] 	= (Production) {.exp = {.items = {{.rule = METHODE, .type = RULE}}, .count = 1}, .action = action_stub};

	//DECL_IDENT productions
	prod_table[DECL_IDENT][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = CONST_TAIL, .type = TOKEN}}, .count = 1}, .action = action_stub};
	prod_table[DECL_IDENT][TOKEN_BOOLEAN] 	= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 3}, .action = action_stub};
	prod_table[DECL_IDENT][TOKEN_INT] 		= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 3}, .action = action_stub};
	prod_table[DECL_IDENT][TOKEN_VOID] 		= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 3}, .action = action_stub};

	//CONST_TAIL productions
	prod_table[CONST_TAIL][TOKEN_LPAR] = (Production) {.exp = {.items = {{.token = TOKEN_LPAR, .type = TOKEN}, {.rule = PARAMS_OPT, .type = RULE}, {.token = TOKEN_RPAR, .type =  TOKEN}, {.token = TOKEN_LACC, .type = TOKEN}, {.rule = STMT_LIST, .type = RULE}, {.token = TOKEN_RACC, .type = TOKEN}}, .count = 6}, .action = action_stub};

	//METHODE productions
	prod_table[METHODE][TOKEN_LPAR]  = (Production) {.exp = {.items = {{.token = TOKEN_LPAR, .type =  TOKEN}, {.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = RULE}, {.rule = CONST, .type = RULE}}, .count = 4}, .action = action_stub};

	//CONST productions
	prod_table[CONST][TOKEN_ID] = (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule =  CONST_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};

	//PARAMS_OPT productions
	prod_table[PARAMS_OPT][TOKEN_RPAR] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[PARAMS_OPT][TOKEN_ID] 		= (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[PARAMS_OPT][TOKEN_BOOLEAN] 	= (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[PARAMS_OPT][TOKEN_INT] 		= (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[PARAMS_OPT][TOKEN_VOID]  	= (Production) {.exp = {.items = {{.rule = PARAMS, .type = RULE}}, .count = 1}, .action = action_stub};

	//PARAMS productions
	prod_table[PARAMS][TOKEN_ID] 		= (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[PARAMS][TOKEN_BOOLEAN] 	= (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[PARAMS][TOKEN_INT] 		= (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[PARAMS][TOKEN_VOID] 		= (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = PARAMS2, .type = RULE}}, .count = 3}, .action = action_stub};

	//PARAMS2 productions
	prod_table[PARAMS2][TOKEN_RPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[PARAMS2][TOKEN_VG] 	= (Production) {.exp = {.items = {{.token = TOKEN_VG, .type = TOKEN}, {.rule = PARAMS, .type = RULE}}, .count = 2}, .action = action_stub};

	//TYPE productions
	prod_table[TYPE][TOKEN_ID] 		= (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}}, .count = 1}, .action = action_stub};
	prod_table[TYPE][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[TYPE][TOKEN_INT] 	= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[TYPE][TOKEN_VOID] 	= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}}, .count = 1}, .action = action_stub};

	//TYPE_DEF productions
	prod_table[TYPE_DEF][TOKEN_BOOLEAN] = (Production) {.exp = {.items = {{.token = TOKEN_BOOLEAN, .type = TOKEN}}, .count = 1}, .action = action_stub};
	prod_table[TYPE_DEF][TOKEN_INT]		= (Production) {.exp = {.items = {{.token = TOKEN_INT, .type = TOKEN}}, .count = 1}, .action = action_stub};
	prod_table[TYPE_DEF][TOKEN_VOID] 	= (Production) {.exp = {.items = {{.token = TOKEN_VOID, .type = TOKEN}}, .count = 1}, .action = action_stub};

	//EXPR_OPT productions
	prod_table[EXPR_OPT][TOKEN_PTSVG] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EXPR_OPT][TOKEN_RPAR] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EXPR_OPT][TOKEN_ID] 			= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR_OPT][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR_OPT][TOKEN_BINOP_SUB] 	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR_OPT][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR_OPT][TOKEN_NUMBER] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR_OPT][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub}; 
	prod_table[EXPR_OPT][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub}; 
	prod_table[EXPR_OPT][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub}; 
	prod_table[EXPR_OPT][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub}; 
	prod_table[EXPR_OPT][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub}; 
	prod_table[EXPR_OPT][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub}; 
	prod_table[EXPR_OPT][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}}, .count = 1}, .action = action_stub}; 

	//EXPR productions
	prod_table[EXPR][TOKEN_ID] 			= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_BINOP_SUB] 	= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_NUMBER] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[EXPR][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = ASSIGN, .type = RULE}}, .count = 1}, .action = action_stub};
	
	// ASSIGN productions
	prod_table[ASSIGN][TOKEN_ID] 		= (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = ASSIGN_ID, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_BINOP_SUB] = (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_FALSE] 	= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	
	//ASSIGN_ID productions
	prod_table[ASSIGN_ID][TOKEN_PTSVG] 		= (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN_ID][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN_ID][TOKEN_RPAR] 		= (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN_ID][TOKEN_VG] 		= (Production) {.exp = {.items = {{.rule = POSTFIX_TAIL, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[ASSIGN_ID][TOKEN_BINOP_EQ] 	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_EQ, .type = TOKEN}, {.rule = EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ASSIGN_ID][TOKEN_PTS] 		= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};

	//LOR_EXPR productions
	prod_table[LOR_EXPR][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_BINOP_SUB] 	= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_NUMBER] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LOR_EXPR][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = LAND_EXPR, .type = RULE},{.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	
	//LOR_EXPR_SUITE productions	
	prod_table[LOR_EXPR_SUITE][TOKEN_PTSVG] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LOR_EXPR_SUITE][TOKEN_RPAR] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LOR_EXPR_SUITE][TOKEN_VG] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LOR_EXPR_SUITE][TOKEN_BINOP_OR] 	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_OR, .type = TOKEN},{.rule = LAND_EXPR, .type = RULE},{.rule = LOR_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	//LAND_EXPR productions
	prod_table[LAND_EXPR][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_BINOP_SUB]  = (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_NUMBER] 	= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LAND_EXPR][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};


	// ADD productions
	prod_table[ADD][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_OPP]			= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_STR]	 		= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_TRUE]			= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_FALSE]		= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_NEW] 			= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[ADD][TOKEN_LPAR]			= (Production) {.exp = {.items = {{.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// ADD_SUITE productions
	prod_table[ADD_SUITE][TOKEN_PTSVG] 			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_RPAR] 			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_VG] 			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_OR] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_AND] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_2EQUAL]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_DIFF] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_INF] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_INFEG]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_SUP] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_SUPEG]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_INSTANCEOF]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_EOF]			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_ADD]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_ADD, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[ADD_SUITE][TOKEN_BINOP_SUB]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUB, .type = TOKEN}, {.rule = MULT, .type = RULE}, {.rule = ADD_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// MULT productions
	prod_table[MULT][TOKEN_LPAR]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_OPP]			= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_STR]			= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_FALSE]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_THIS]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_NULL]		= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[MULT][TOKEN_NEW]			= (Production) {.exp = {.items = {{.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// MULT_SUITE productions
	prod_table[MULT_SUITE][TOKEN_PTSVG] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_RPAR] 			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_VG] 			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_OR] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_AND] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_2EQUAL]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_DIFF] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_INF] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_INFEG]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_SUP] 	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_SUPEG]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_INSTANCEOF]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_EOF]			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_ADD]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_SUB]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_MULT]	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MULT,.type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_DIV]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_DIV, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[MULT_SUITE][TOKEN_BINOP_MOD]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_MOD, .type = TOKEN}, {.rule = UNARY, .type = RULE}, {.rule = MULT_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};


	// UNARY productions
	prod_table[UNARY][TOKEN_LPAR]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.token = TOKEN_UNOP_SUB, .type = TOKEN}, {.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[UNARY][TOKEN_OPP]		= (Production) {.exp = {.items = {{.token = TOKEN_OPP, .type = TOKEN},  	{.rule = UNARY, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[UNARY][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_FALSE]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_THIS]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_NULL]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[UNARY][TOKEN_NEW]		= (Production) {.exp = {.items = {{.rule = C_O_P, .type = RULE}}, .count = 1}, .action = action_stub};

	// C_O_P productions
	prod_table[C_O_P][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[C_O_P][TOKEN_STR]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[C_O_P][TOKEN_TRUE]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[C_O_P][TOKEN_FALSE]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[C_O_P][TOKEN_THIS]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[C_O_P][TOKEN_NULL]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[C_O_P][TOKEN_NEW]	= (Production) {.exp = {.items = {{.rule = POSTFIX_EXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[C_O_P][TOKEN_LPAR]	= (Production) {.exp = {.items = {{.token = TOKEN_LPAR, .type = TOKEN}, {.rule = C_O_P_PAR, .type = RULE}}, .count = 2}, .action = action_stub};

	// POSTFIX_EXPR productions
	prod_table[POSTFIX_EXPR][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[POSTFIX_EXPR][TOKEN_TRUE]	= (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[POSTFIX_EXPR][TOKEN_FALSE]	= (Production) {.exp = {.items = {{.rule = PRIMARY, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};

	// POSTFIX_TAIL productions
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_MULT]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_DIV]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_MOD]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_ADD]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[POSTFIX_TAIL][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[POSTFIX_TAIL][TOKEN_EOF]			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	
	// PRIMARY productions
	prod_table[PRIMARY][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.token = TOKEN_NUMBER,.type = TOKEN}}, .count = 1}, .action = action_stub};
	prod_table[PRIMARY][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.token = TOKEN_TRUE,  .type = TOKEN}}, .count = 1}, .action = action_stub};
	prod_table[PRIMARY][TOKEN_FALSE]	= (Production) {.exp = {.items = {{.token = TOKEN_FALSE, .type = TOKEN}}, .count = 1}, .action = action_stub};

	// BLOCK productions
	prod_table[BLOCK][TOKEN_LACC]	= (Production) {.exp = {.items = {{.token = TOKEN_LACC,.type = TOKEN}, {.rule = STMT_LIST,.type = RULE}, {.token = TOKEN_RACC,.type = TOKEN}}, .count = 3}, .action = action_stub};

	// STMT_IF productions
	prod_table[STMT_IF][TOKEN_ELSE]		= (Production) {.exp = {.items = {{.token = TOKEN_ELSE, .type = TOKEN}, {.rule = BLOCK, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_RETURN]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_FOR]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_IF]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_NEW]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_NULL]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_THIS]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_FALSE]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_TRUE]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_STR]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_NUMBER]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_OPP]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_BINOP_SUB]= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_BOOLEAN]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_INT]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_VOID]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_PTSVG]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_LPAR]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_ID]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_RACC]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[STMT_IF][TOKEN_LACC]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// STMT_TYPE_IDENT productions
	prod_table[STMT_TYPE_IDENT][TOKEN_PTSVG] 	= (Production) {.exp = {.items = {{.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 0}, .action = action_stub};
	prod_table[STMT_TYPE_IDENT][TOKEN_BINOP_EQ] = (Production) {.exp = {.items = {{.token = TOKEN_BINOP_EQ, .type = TOKEN}, {.rule = EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 3}, .action = action_stub};

	// STMT_IDENT productions
	prod_table[STMT_IDENT][TOKEN_ID] 		= (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_IDENT][TOKEN_PTSVG] 	= (Production) {.exp = {.items = {{.rule = ASSIGN_ID, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT_IDENT][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = ASSIGN_ID, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT_IDENT][TOKEN_BINOP_EQ] 	= (Production) {.exp = {.items = {{.rule = ASSIGN_ID, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT_IDENT][TOKEN_PTS] 		= (Production) {.exp = {.items = {{.rule = ASSIGN_ID, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};

	// STMT productions
	prod_table[STMT][TOKEN_ID] 			= (Production) {.exp = {.items = {{.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_IDENT, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_LACC] 		= (Production) {.exp = {.items = {{.rule = BLOCK, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[STMT][TOKEN_PTSVG] 		= (Production) {.exp = {.items = {{.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_BOOLEAN] 	= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[STMT][TOKEN_INT] 		= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[STMT][TOKEN_VOID] 		= (Production) {.exp = {.items = {{.rule = TYPE_DEF, .type = RULE}, {.token = TOKEN_ID, .type = TOKEN}, {.rule = STMT_TYPE_IDENT, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[STMT][TOKEN_BINOP_SUB] 	= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_NUMBER] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = LOR_EXPR, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[STMT][TOKEN_IF] 			= (Production) {.exp = {.items = {{.token = TOKEN_IF, .type = TOKEN}, {.token = TOKEN_LPAR, .type = TOKEN}, {.rule = EXPR, .type = RULE}, {.token = TOKEN_LPAR, .type = TOKEN}, {.rule = BLOCK, .type = RULE}, {.rule = STMT_IF, .type = RULE}}, .count = 6}, .action = action_stub};
	prod_table[STMT][TOKEN_FOR] 		= (Production) {.exp = {.items = {{.token = TOKEN_FOR, .type = TOKEN}, {.token = TOKEN_LPAR, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_LPAR, .type = TOKEN}, {.rule = STMT, .type = RULE}}, .count = 9}, .action = action_stub};
	prod_table[STMT][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.token = TOKEN_RETURN, .type = TOKEN}, {.rule = EXPR_OPT, .type = RULE}, {.token = TOKEN_PTSVG, .type = TOKEN}}, .count = 3}, .action = action_stub};

	// STMT_LIST productions
	prod_table[STMT_LIST][TOKEN_ID] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_LACC] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_PTSVG] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_LACC] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_BOOLEAN] 	= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_INT] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_VOID] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_BINOP_SUB] 	= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_NUMBER] 	= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_ID] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_IF] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_ELSE] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_FOR] 		= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_RETURN] 	= (Production) {.exp = {.items = {{.rule = STMT, .type = RULE}, {.rule = STMT_LIST, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[STMT_LIST][TOKEN_RACC] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// LEXPR productions
	prod_table[LEXPR_EXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LEXPR_EXPR][TOKEN_LPAR] = (Production) {.exp = {.items = {{.token = TOKEN_VG, .type = TOKEN}, {.rule = LEXPR, .type = RULE}}, .count = 2}, .action = action_stub};

	// LEXPR productions
	prod_table[LEXPR][TOKEN_ID] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_BINOP_SUB] 	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_NUMBER] 	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_STR] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_TRUE] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[LEXPR][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.rule = LEXPR_EXPR, .type = RULE}}, .count = 2}, .action = action_stub};

	// LEXPR_OPT productions
	prod_table[LEXPR_OPT][TOKEN_ID] 		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_LPAR] 		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_BINOP_SUB]	= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_NUMBER]		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_STR]		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_FALSE] 		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_THIS] 		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_NULL] 		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = LEXPR, .type = RULE}}, .count = 1}, .action = action_stub};
	prod_table[LEXPR_OPT][TOKEN_RPAR] 		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};

	// PRIMARY_PAR productions
	prod_table[PRIMARY_PAR][TOKEN_ID] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_LPAR] 	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_BINOP_SUB]= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_STR]		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_FALSE] 	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_THIS] 	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_NULL] 	= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[PRIMARY_PAR][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};

	// EQ_EXPR productions
	prod_table[EQ_EXPR][TOKEN_LPAR] 	= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_BINOP_SUB]= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_STR]		= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_FALSE] 	= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_THIS] 	= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_NULL] 	= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};
	prod_table[EQ_EXPR][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = RELATIONAL_EXPR, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}}, .count = 2}, .action = action_stub};

	// EQ_EXPR_SUITE productions
	prod_table[EQ_EXPR_SUITE][TOKEN_VG]			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EQ_EXPR_SUITE][TOKEN_RPAR]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EQ_EXPR_SUITE][TOKEN_PTSVG]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_AND]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_OR]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_2EQUAL]	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_2EQUAL, .type = TOKEN}, {.rule = RELATIONAL_EXPR,.type = RULE}, {.token = EQ_EXPR_SUITE, .type = TOKEN}}, .count = 3}, .action = action_stub};
	prod_table[EQ_EXPR_SUITE][TOKEN_BINOP_DIFF]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_DIFF, .type = TOKEN}, {.rule = RELATIONAL_EXPR,.type = RULE}, {.token = EQ_EXPR_SUITE, .type = TOKEN}}, .count = 3}, .action = action_stub};

	// RELATIONAL_EXPR productions
	prod_table[RELATIONAL_EXPR][TOKEN_LPAR]		= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_BINOP_SUB]= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_OPP] 		= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_STR]		= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_TRUE]		= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_FALSE] 	= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_THIS] 	= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_NULL] 	= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[RELATIONAL_EXPR][TOKEN_NEW] 		= (Production) {.exp = {.items = {{.rule = ADD, .type = RULE}, {.rule = RELATIONAL_EXPR_SUITE, .type = RULE}}, .count = 2}, .action = action_stub};

	// RELATIONAL_EXPR_SUITE productions
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_VG]			= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_RPAR]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_PTSVG]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_2EQUAL]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_DIFF]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_AND]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_OR]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_INF]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_INF, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.token = RELATIONAL_EXPR_SUITE, .type = TOKEN}}, .count = 3}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_INFEG]	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_INFEG, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.token = RELATIONAL_EXPR_SUITE, .type = TOKEN}}, .count = 3}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_SUP]		= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUP, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.token = RELATIONAL_EXPR_SUITE, .type = TOKEN}}, .count = 3}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_BINOP_SUPEG]	= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_SUPEG, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.token = RELATIONAL_EXPR_SUITE, .type = TOKEN}}, .count = 3}, .action = action_stub};
	prod_table[RELATIONAL_EXPR_SUITE][TOKEN_INSTANCEOF]		= (Production) {.exp = {.items = {{.token = TOKEN_INSTANCEOF, .type = TOKEN}, {.rule = ADD, .type = RULE}, {.token = RELATIONAL_EXPR_SUITE, .type = TOKEN}}, .count = 3}, .action = action_stub};

	// LAND_EXPR_SUITE productions
	prod_table[LAND_EXPR_SUITE][TOKEN_VG]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LAND_EXPR_SUITE][TOKEN_RPAR]		= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LAND_EXPR_SUITE][TOKEN_PTSVG]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LAND_EXPR_SUITE][TOKEN_BINOP_OR]	= (Production) {.exp = {.items = {}, .count = 0}, .action = action_stub};
	prod_table[LAND_EXPR_SUITE][TOKEN_BINOP_AND]= (Production) {.exp = {.items = {{.token = TOKEN_BINOP_AND, .type = TOKEN}, {.rule = EQ_EXPR, .type = RULE}, {.rule = LAND_EXPR_SUITE, .type = RULE}}, .count = 3}, .action = action_stub};

	// C_O_P_PAR productions
	prod_table[C_O_P_PAR][TOKEN_BOOLEAN]= (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = C_O_P, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_INT]	= (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = C_O_P, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_VOID]	= (Production) {.exp = {.items = {{.rule = TYPE, .type = RULE}, {.token = TOKEN_RPAR, .type = TOKEN}, {.rule = C_O_P, .type = RULE}}, .count = 3}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_ID]		= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_LPAR]	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_OPP] 	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_NUMBER]	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_STR]	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_TRUE]	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_FALSE] 	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_THIS] 	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_NULL] 	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};
	prod_table[C_O_P_PAR][TOKEN_NEW] 	= (Production) {.exp = {.items = {{.rule = PRIMARY_PAR, .type = RULE}, {.rule = POSTFIX_TAIL, .type = RULE}}, .count = 2}, .action = action_stub};

}

static AST* create_ast_token(Token token, Lexer* lex)
{
	// printf("Creating AST for token type: %d\n", token.type);

	switch (token.type) {
		case TOKEN_NUMBER:
			return create_number_node(token_number(lex, token));
		case TOKEN_TRUE:
			return create_number_node(1);
		case TOKEN_FALSE:
			return create_number_node(0);
		case TOKEN_BINOP_ADD:
			return create_binary_op_node('+', NULL, NULL);
		case TOKEN_BINOP_SUB:
			return create_binary_op_node('-', NULL, NULL);
		case TOKEN_BINOP_MULT:
			return create_binary_op_node('*', NULL, NULL);
		case TOKEN_BINOP_DIV:
			return create_binary_op_node('/', NULL, NULL);
		case TOKEN_BINOP_MOD:
			return create_binary_op_node('%', NULL, NULL);
		case TOKEN_OPP:
			return create_unary_op_node('!', NULL);
		case TOKEN_UNOP_ADD:
			return create_unary_op_node('+', NULL);
		case TOKEN_UNOP_SUB:
			return create_unary_op_node('-', NULL);
		default:
			return NULL;
	}
}


AST* parse_main(Lexer* lex, int debug)
{
	init_prod_table();

	stk = stack_init();
	astk = ast_stack_init(); 	// the astk is global now deal wif it ...

	stack_push(stk, (NUT){.type = RULE, .rule = FIL});


	while (stk->top > 0) {
		if (debug) {
			printf("==================== NEW ITERATION ===================\n"); 
			print_parser(stk);
			printf("\n");
			token_pprint(lex->tok, lex);
			printf("\n");
			print_ast_parser(astk);
			printf("\n");
		}
		
		NUT curr = stack_pop(stk);
		Token current_token = lexer_giveToken(lex);

		switch (curr.type) {
			case RULE: {
				Production prod = prod_table[curr.rule][current_token.type];
				
				// Check if production exists (internal error)
				if (prod.exp.count == 0 && prod.action == NULL) {
					parser_error("No production for rule %d and token %d", curr.rule, current_token.type);
					exit(1);
				}

				for (int i = prod.exp.count - 1; i >= 0; i--) {
					stack_push(stk, prod.exp.items[i]);
				}

				if (prod.action) {
					prod.action();
				}
				break;
			}
			case TOKEN: {
				if (curr.token == TOKEN_EOF && curr.token == current_token.type)
					break;

				if (token_type_equality(curr.token, current_token)) { 
					AST* token_node = create_ast_token((Token){.position = current_token.position, .type = curr.token}, lex);
					if (token_node) {
						ast_stack_push(astk, token_node);
					}
					lexer_next(lex);
				} else {
					parser_error("Expected token %d, got %d", curr.token, current_token.type);
					exit(1);
				}
				break;
			}
		}
	}

	// TODO: remove after actions are implemented
	// if (astk->top != 1) {
	// 	parser_error("Parse finished with %lu AST nodes (expected 1)", astk->top);
	// 	exit(1);
	// }

	AST* result = ast_stack_pop(astk);
	free(stk); 
	free(astk);
	return result;
}
