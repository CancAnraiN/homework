#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolT.h"

struct table *Symbol_Table;

int indexNum = 0;

void create()
{
	
	Symbol_Table = malloc(sizeof(Symbol_Table));
	// if dont add --------
	//invalid conversion 'void*' to 'Symbol_Table'
}
int lookup(char *s)
{
	int i;
	for(i = 0 ; i < indexNum; i++)
	{ 
		if(strcmp(Symbol_Table[i].ID,s) == 0) return i;
	}
	return -1;
}
int insert(char *s)
{	
	
	if( lookup(s) == -1)
	{
		strcpy(Symbol_Table[indexNum].ID,s);
		indexNum++;
		return indexNum;
	}
	else return indexNum;
	

	return -1;
}
void dump()
{
	int temp = indexNum;
	printf("The symbol table contains:\n");
	while(temp >= 0)
	{
		printf("%s\n",Symbol_Table[temp].ID);
		temp--;
	}
}

