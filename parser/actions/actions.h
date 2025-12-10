#ifndef ACTIONS_H
#define ACTIONS_H

#include <parser/backend_parser.h>


#define MAXIMUM_EXPANSION 10


extern ASTStack* astk;
extern Stack* stk;

typedef struct {
	NUT items[MAXIMUM_EXPANSION]; 	// max is 10 for now
	int count;
} Expansion;

typedef struct {
	Expansion exp;
	void (*action)(Lexer*);		// an action is an operation on the AST stack	
} Production;

void action_stub(Lexer* lex); 
void action_unop_create(Lexer* lex);
void action_binop_create(Lexer* lex);

void action_unop_end(Lexer* lex);
void action_binop_end(Lexer* lex);


void action_create_number_leaf(Lexer* lex);
void action_create_boolean_leaf(Lexer* lex);

#endif