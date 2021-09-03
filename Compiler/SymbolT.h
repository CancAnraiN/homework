#ifndef _SYMBOL_T_H
#define _SYMBOL_T_H

struct table{
	char ID[50];
};

void create();
int lookup(char *s);
int insert(char *s);
void dump();

#endif
