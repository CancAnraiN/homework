%error-verbose
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
extern int position,line;
extern int errtype;
extern char errID1[50];
extern char errID2[50];
int id = 0;	//trace id number

char symbolTable[50][50];
char classTable[50][50];  //store "class's name"
int classnum;
char funcTable[50][50];   // store "func.'s name"
int funcnum;


int LCindexlist[50],LCindex;	//variable scope layer
int declaration = 0;

void yyerror(const char*);
void ClassName(char *s);
void FunctionName(char *s);
void IdName(char *s);
void CheckId(char *s);	
int Look(char *s);
int LookAll(char *s);

%}

%union{
char* strval;
}
%token BOOLEAN BREAK BYTE CASE CHAR CATCH CLASS CONST CONTINUE DEFAULT DO DOUBLE ELSE EXTENDS FALSE FINAL FINALLY FLOAT FOR IF IMPLEMENTS INT LONG MAIN NEW PRINT PRIVATE PROTECTED PUBLIC RETURN READ SHORT STATIC STRING SWITCH THIS TRUE TRY VOID WHILE
%token COMMA COLON SEMI LP RP LC RC LB RB DOT
%token DADD DSUB EQ BEQ LEQ NEQ AND OR NOT ADD SUB MUL DIV MOD ASSIGN LT BT
%token FNUM STR NUM CH
%token <strval> ID
%start PRO
%%

//class
PRO: class_list|PRO class_list|error;

check_class: CLASS ID {
	id = 0;
	ClassName($2);
	id++;
};
//scope layer
check_LC: LC {
		LCindexlist[LCindex++] = id;
};

class_list: check_class check_LC element_list RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
}
| check_class check_LC RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
}; 

element_list: element | element_list element;
element: method | variable;

variable: STATIC type id_list SEMI
| CONST type id_list SEMI
| FINAL type id_list SEMI
| type id_list SEMI
| type LB RB ID ASSIGN NEW type LB NUM RB SEMI{
	IdName($4);
	id++;
}
| ID ID ASSIGN NEW ID LP RP SEMI{
	CheckId($1);
	if(errtype != 2)
		IdName($2);
	id++;
};

type: INT|FLOAT|BOOLEAN|CHAR|STRING|LONG|SHORT|VOID|DOUBLE;

id_list: id_expr|id_list COMMA id_expr;
id_expr: ID{
	IdName($1);
	id++;
}
| ID ASSIGN expr{	
	IdName($1);
	id++;
};

method: modifier type ID LP argu_list RP compound{
	IdName($3);
	id++;
}
|check_functionName LP argu_list RP compound
//void main()	
|VOID MAIN LP argu_list RP compound
//main()	
|MAIN LP argu_list RP compound;		
check_functionName: type ID{	
	FunctionName($2);
	id++;
};

modifier: PUBLIC|PROTECTED|PRIVATE;
//arguments in method(seprate by comma)
argu_list: argu|argu_list COMMA argu;
argu: type ID{
	IdName($2);
	id++;
}
|
;

//make sure variables' and functions' scope 
compound: check_LC statement_list RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
}
|check_LC RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
};

statement_list: statement|statement_list statement;
statement: variable|compound|basic|condition|for|return|method_invocation|while semi;
semi:SEMI|PRINT;


basic: name ASSIGN expr SEMI
|PRINT LP expr RP SEMI
|READ LP name RP SEMI
|name DADD SEMI
|name DSUB SEMI
|expr SEMI
|ID ASSIGN expr SEMI;


name: ID DOT ID{
	CheckId($1);
};
expr: term|expr ADD term|expr SUB term;
term: factor|term MUL factor|term DIV factor;
factor: ID{
	CheckId($1);
}|const_expr
|LP expr RP
|prefixOp ID{
	CheckId($2);
}|ID postfixOp{
	CheckId($1);
}|ID LB expr RB{
	CheckId($1);
}|ID LB expr RB postfixOp{	
	CheckId($1);
}|prefixOp ID LB expr RB{
	CheckId($2);
}
;
prefixOp: DADD|DSUB|ADD|SUB; 	// ++/--/+/- (+,- only appear behind)
postfixOp:DADD|DSUB;		// ++/--
const_expr: NUM|FNUM|STR;
//function call
method_invocation: name LP expr_list RP|name LP RP; // Check(a,b,c) && Check()
expr_list: expr|expr_list COMMA expr; // a or a,b,....


condition: IF LP bool_expr RP statement else;
else: ELSE statement;
bool_expr: expr infixOp expr; // a =/!=/</>/<=/>=/ b
infixOp:EQ|NEQ|LT|BT|LEQ|BEQ;


//loop:
while: WHILE LP bool_expr RP statement|error RC;	
for:FOR LP forPart1 SEMI bool_expr SEMI forPart3 RP statement;	
forPart1: forInit|forPart1 COMMA forInit; //for("this part";...;...)
forInit: ID ASSIGN expr{
	CheckId($1);
}|INT ID ASSIGN expr{
	declaration = 1;
	IdName($2);
	id++;
}|ID LB expr RB ASSIGN expr{
	CheckId($1);
}
;
forPart3: ID DADD{		 //for(...;...;"this part")
	CheckId($1);
}
|ID DSUB{
	CheckId($1);
}
|ID LB expr RB DADD{		 //a[10]++ for example(in test6)
	CheckId($1);
}
|ID LB expr RB DSUB{		 //a[10]-- for example(in test6)
	CheckId($1);
}
;

return: RETURN expr SEMI;        //return

%%

void yyerror(const char *str)
{
	fprintf(stderr,"Line %d: 1st char %d has %s\n",line,position,str+14);
}
int Look(char *s)
{
	for(int i=0;i < classnum;i++)
	{
		if(strcmp(s,classTable[i]) == 0)
			return 0;
	}
	for(int j = LCindexlist[LCindex-1]; j < id; j++)
	{
		if(strcmp(s,symbolTable[j]) == 0)
			return 0;
	}
	return 1;	
}
int LookAll(char *s)	
{
	for(int i=0;i < classnum; i++)
	{
		if(strcmp(s,classTable[i]) == 0)
			return 0;
	}
	for(int j=0;j < id;j++)
	{
		if(strcmp(s,symbolTable[j]) == 0)
			return 0;
	}
	return 1;
}
void CheckId(char *s)	// check id if duplicated,so need to check all	
{
	if(LookAll(s))
	{
		errtype = 2;
		strcpy(errID2,s);
	}
}
void ClassName(char *s)
{
	if(Look(s))
	{
		strcpy(classTable[classnum],s);
		classnum++;
		strcpy(symbolTable[id],s);
	}
	else
	{
		errtype = 1;
		strcpy(errID1,s);
		id--;
	}
}
void FunctionName(char *s)
{
	if(Look(s))
	{
		strcpy(funcTable[funcnum],s);
		funcnum++;
		strcpy(symbolTable[id],s);
	}
	else
	{
		errtype = 1;
		strcpy(errID1,s);
		id--;
	}
}
void IdName(char *s)
{
	if(declaration == 1)
		strcpy(symbolTable[id],s);
	else
	{
		if(Look(s))
			strcpy(symbolTable[id],s);
		else
		{
			errtype = 1;
			strcpy(errID1,s);
			id--;
		}
	}
}

int main()
{
	yyparse();
	return 0;
}
