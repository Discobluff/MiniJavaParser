#ifndef PROD_TABLE_H
#define PROD_TABLE_H

#include <parser/parse.h>


extern Production prod_table[RULE_SIZE][TOKENTYPE_SIZE];

void init_prod_table(void);

#endif
