#ifndef ACTIONS_H
#define ACTIONS_H

#include "../backend_parser.h"


#define MAXIMUM_EXPANSION 10


extern ASTStack* astk;
extern Stack* stk;

typedef struct {
	NUT items[MAXIMUM_EXPANSION]; 	// max is 10 for now
	int count;
} Expansion;

typedef struct {
	Expansion exp;
	void (*action)(void);		// an action is an operation on the AST stack	
} Production;

void action_stub(void); 
void action_suite_unop(void);
void action_suite_binop(void);

#endif