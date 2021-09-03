%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"y.tab.h"


int position = 1,line = 1;
int errtype;
char errID1[50];
char errID2[50];
void commentline(char *s);

%}
SYM [\,\:\;\(\)\{\}\[\]]

OPERATOR "++"|"--"|"=="|">="|"<="|"!="|"&&"|"||"|"!"|[\+\-\*\/%=<>]

RESERVE boolean|break|byte|case|char|catch|class|const|continue|default|do|double|else|extends|false|final|finally|float|for|if|implements|int|long|main|new|(print)|private|protected|public|return|short|static|string|switch|this|true|try|void|while

ID [a-zA-Z_\$]+[0-9a-zA-Z_\$]*
ID_ERR [0-9\^\#]+[a-zA-Z]+

INT [\-]*[0-9]+
FLOAT [+-]?[0-9]+(\.[0-9]+)?([Ee][+-]?[0-9]+)?

STR_ERR \"([^\"\r\n])*
STR \"([^\\\"\r\n]|[^\r\n])*\"
CHARACTER ['][^']*[']

SPACE [ \t]
NEW \r?\n

COMMENT ("//".*)|"/*"([^*]|\*+[^*/])*\*+"/"

%%


{SYM} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	if(strcmp(yytext,",") == 0)
		return COMMA;
	else if(strcmp(yytext,":") == 0)
		return COLON;
	else if(strcmp(yytext,";") == 0)
		return SEMI;
	else if(strcmp(yytext,"(") == 0)
		return LP;
	else if(strcmp(yytext,")") == 0)
		return RP;
	else if(strcmp(yytext,"{") == 0)
		return LC;
	else if(strcmp(yytext,"}") == 0)
		return RC;
	else if(strcmp(yytext,"[") == 0)
		return LB;
	else if(strcmp(yytext,"]") == 0)
		return RB;
	else if(strcmp(yytext,".") == 0)
		return DOT;
}


{OPERATOR} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	if(strcmp(yytext,"++") == 0)
		return DADD;
	else if(strcmp(yytext,"--") == 0)
		return DSUB;
	else if(strcmp(yytext,"==") == 0)
		return EQ;
	else if(strcmp(yytext,">=") == 0)
		return BEQ;
	else if(strcmp(yytext,"<=") == 0)
		return LEQ;
	else if(strcmp(yytext,"!=") == 0)
		return NEQ;
	else if(strcmp(yytext,"&&") == 0)
		return AND;
	else if(strcmp(yytext,"||") == 0)
		return OR;
	else if(strcmp(yytext,"!") == 0)
		return NOT;
	else if(strcmp(yytext,"+") == 0)
		return ADD;
	else if(strcmp(yytext,"-") == 0)	
		return SUB;
	else if(strcmp(yytext,"*") == 0)
		return MUL;
	else if(strcmp(yytext,"/") == 0)
		return DIV;
	else if(strcmp(yytext,"%") == 0)
		return MOD;
	else if(strcmp(yytext,"=") == 0)
		return ASSIGN;
	else if(strcmp(yytext,"<") == 0)
		return LT;
	else if(strcmp(yytext,">") == 0)
		return BT;
}

{RESERVE} { 
	if(position == 1)	
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	if(strcmp(yytext,"boolean") == 0)
		return BOOLEAN;
	else if(strcmp(yytext,"break") == 0)
		return BREAK;
	else if(strcmp(yytext,"byte") == 0)
		return BYTE;
	else if(strcmp(yytext,"case") == 0)
		return CASE;
	else if(strcmp(yytext,"char") == 0)
		return CHAR;
	else if(strcmp(yytext,"catch") == 0)
		return CATCH;
	else if(strcmp(yytext,"class") == 0)
		return CLASS;
	else if(strcmp(yytext,"const") == 0)
		return CONST;
	else if(strcmp(yytext,"continue") == 0)
		return CONTINUE;
	else if(strcmp(yytext,"default") == 0)
		return DEFAULT;
	else if(strcmp(yytext,"do") == 0)
		return DO;
	else if(strcmp(yytext,"double") == 0)
		return DOUBLE;
	else if(strcmp(yytext,"else") == 0)
		return ELSE;
	else if(strcmp(yytext,"extends") == 0)
		return EXTENDS;
	else if(strcmp(yytext,"false") == 0)
		return FALSE;
	else if(strcmp(yytext,"final") == 0)
		return FINAL;
	else if(strcmp(yytext,"finally") == 0)
		return FINALLY;
	else if(strcmp(yytext,"float") == 0)
		return FLOAT;
	else if(strcmp(yytext,"for") == 0)
		return FOR;
	else if(strcmp(yytext,"if") == 0)
		return IF;
	else if(strcmp(yytext,"implements") == 0)
		return IMPLEMENTS;
	else if(strcmp(yytext,"int") == 0)
		return INT;
	else if(strcmp(yytext,"long") == 0)
		return LONG;
	else if(strcmp(yytext,"main") == 0)
		return MAIN;
	else if(strcmp(yytext,"new") == 0)
		return NEW;
	else if(strcmp(yytext,"print") == 0)
		return PRINT;
	else if(strcmp(yytext,"protected") == 0)
		return PROTECTED;
	else if(strcmp(yytext,"private") == 0)
		return PRIVATE;
	else if(strcmp(yytext,"public") == 0)
		return PUBLIC;
	else if(strcmp(yytext,"return") == 0)
		return RETURN;
	else if(strcmp(yytext,"read") == 0)
		return READ;
	else if(strcmp(yytext,"short") == 0)
		return SHORT;
	else if(strcmp(yytext,"static") == 0)
		return STATIC;
	else if(strcmp(yytext,"string") == 0)
		return STRING;
	else if(strcmp(yytext,"switch") == 0)
		return SWITCH;
	else if(strcmp(yytext,"this") == 0)
		return THIS;
	else if(strcmp(yytext,"true") == 0)
		return TRUE;
	else if(strcmp(yytext,"try") == 0)
		return TRY;
	else if(strcmp(yytext,"void") == 0)
		return VOID;
	else if(strcmp(yytext,"while") == 0)
		return WHILE;
}

{ID_ERR} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
}

{ID} {

	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	yylval.strval = strdup(yytext);
	return ID;
}

{INT} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	return NUM;
}

{FLOAT} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	return FNUM;
}

{STR_ERR} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
}

{STR} { 
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	return STR;
}

{CHARACTER} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	return CH;
}

{NEW} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("\n");
	line++;
	position = 1;
	if(errtype == 1)
	{
		printf("> \'%s\' is a duplicate identifier.\n",errID1);
	}
	else if(errtype == 2)
	{
		printf("> \'%s\' hasn't been declared yet.\n",errID2);
	}
	errtype = 0;
}

{SPACE} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
}

{COMMENT} {
	if(position == 1)
		printf("Line %d: ",line);
	printf("%s",yytext);
	position += yyleng;
	commentline(yytext);
}

. {position += yyleng;}

%%

void commentline(char *s)
{
	int i;
	for (i = 0; i < strlen(s); i++)
		if(s[i] == '\n')
			line++;
}

