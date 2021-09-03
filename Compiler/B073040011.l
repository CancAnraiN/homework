%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 50
//#include "SymbolT.h"
unsigned charCount = 1, idCount = 0,lineCount = 1;
char str[50];
struct table{
	char ID[50];
};

struct table *Symbol_Table;

int indexNum = 0;

void create();
int lookup(char *s);
int insert(char *s);
void dump();

%}

Error_ID [0-9\^\#]+[A-Za-z0-9_\$]*

ErrorStr (\'.*\')|(\"\\\")|(\"[^\"]*)|(\".*([^\\][\"])+.*\")

Symbol [\,\:\;\(\)\{\}\[\]]

Operator \+\+|\-\-|\>\=|\<\=|\=\=|\!\=|\&\&|\|\||[\+\-\*\/\=\%\<\!]

Reserved_word boolean|break|byte|case|char|catch|class|const|continue|default|do|double|else|extends|false|final|finally|float|for|if|implements|int|long|main|new|(print)|private|protected|public|return|short|static|string|switch|this|true|try|void|while

ID [A-Za-z_\$][A-Za-z0-9_\$]*


Int [\-]?[0-9]+
Float [\-]?[0-9]+([\.][0-9]+)+([eE][+-]?[0-9]+)?

String [\"](.*([\\][\"])*.*)[\"]


Newline \r?\n
Whitespace [ \t]

Comment1 \/\*.*([\r]?[\n])*.*\*\/
Comment2 (\/\/).*




%%

{Whitespace} {
	charCount++;
}
{Newline} {
	lineCount++;
	charCount = 1;
}
{Operator} {
	printf("Line: %d, 1st char: %d, \"%s\" is an \"Operator\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}
{Symbol} {
	printf("Line: %d, 1st char: %d, \"%s\" is a \"Symbol\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}
{Reserved_word} {
	printf("Line: %d, 1st char: %d, \"%s\" is a \"Reserved_word\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}
{Int} {
	printf("Line: %d, 1st char: %d, \"%s\" is an \"Int\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}
{Float} {
	printf("Line: %d, 1st char: %d, \"%s\" is a \"Float\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}
{Error_ID} {
	printf("Line: %d, 1st char: %d, \"%s\" is an \"Invalid ID\".\n", lineCount, charCount, yytext);
}
{ID} {
	strcpy(str,yytext);
	printf("Line: %d, 1st char: %d, \"%s\" is an \"ID\".\n", lineCount, charCount, yytext);
	insert(str);
	charCount += yyleng;
}

{ErrorStr} {
	printf("Line: %d, 1st char: %d, \"%s\" is a \"Invalid String\".\n", lineCount, charCount, yytext);
}
{String} {
	printf("Line: %d, 1st char: %d, \"%s\" is a \"String\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}
{Comment1} {
	int lineC = 0;
	int i = 0;
	for(i = 0; i < strlen(yytext); i++)
	{
		if(yytext[i] == '\n') lineC++;
	}
	printf("Line: %d, 1st char: %d, \"%s\" is an \"Comment\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
	lineCount += lineC;
}
{Comment2} {
	printf("Line: %d, 1st char: %d, \"%s\" is an \"Comment\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}
. {
	printf("Line: %d, 1st char: %d, \"%s\" is a \"Invalid\".\n", lineCount, charCount, yytext);
	charCount += yyleng;
}


%%

void create()//create N symTab for store ID
{
	Symbol_Table = malloc(N*sizeof(Symbol_Table));
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
		printf("%s\n",s);
		strcpy(Symbol_Table[indexNum].ID,s);
		indexNum++;
		return indexNum;
	}
	else
	{
		printf("Already Have!!\n");
		return indexNum;
	}

	return -1;
}
void dump()
{
	int i = 0;
	printf("The symbol table contains:\n");
	while(i < indexNum)
	{
		printf("%s\n",Symbol_Table[i].ID);
		i++;
	}
}

int main()
{
	create();
	yylex();
	dump();
	return 0;

}

