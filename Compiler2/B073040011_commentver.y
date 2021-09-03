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

//處理class error
PRO: class_list|PRO class_list|error;
//class一開始的開頭
check_class: CLASS ID {
	id = 0;
	ClassName($2);
	id++;
};
//由於要確認scope layer的問題, 所以將'{'另外抓出成一個statement
check_LC: LC {
		LCindexlist[LCindex++] = id;
};
//真正的class的文法
class_list: check_class check_LC element_list RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
}
| check_class check_LC RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
}; 
//class裡可能有的variables及methods
element_list: element | element_list element;
element: method | variable;
//處理變數宣告等, 以及new object的宣告
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
//變數可能的type
type: INT|FLOAT|BOOLEAN|CHAR|STRING|LONG|SHORT|VOID|DOUBLE;
//處理一次宣告多個變數(id用comma隔開)
id_list: id_expr|id_list COMMA id_expr;
id_expr: ID{
	IdName($1);
	id++;
}
| ID ASSIGN expr{	
	IdName($1);
	id++;
};
//java的method類似c的function, 但可能有modifier在method宣告前
method: modifier type ID LP argu_list RP compound{
	IdName($3);
	id++;
}
|check_funcname LP argu_list RP compound	//沒有modifier
|VOID MAIN LP argu_list RP compound	//處理void main()
|MAIN LP argu_list RP compound;		//main()
check_funcname: type ID{	
	FunctionName($2);
	id++;
};

modifier: PUBLIC|PROTECTED|PRIVATE;
//為pass進method的arguments, 可能為多個(用comma分開), 可能沒有
argu_list: argu|argu_list COMMA argu;
argu: type ID{
	IdName($2);
	id++;
}
|
;

//statement1:make sure variables' and functions' scope 
compound: check_LC statement_list RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
}
|check_LC RC{
	id = LCindexlist[LCindex-1];
	LCindex--;
};
//compound裡可能有多個statements
statement_list: statement|statement_list statement;
statement: variable|compound|simple|condition|for|return|method_invocation|while semi;
semi:SEMI|PRINT;

//statement2
simple: name ASSIGN expr SEMI
|PRINT LP expr RP SEMI
|READ LP name RP SEMI
|name DADD SEMI
|name DSUB SEMI
|expr SEMI
|ID ASSIGN expr SEMI;
//由於在name單獨加上ID會有shift reduce conflict, 所以在simple再加上了一個ID ASSIGN expr SEMI
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
}|ID LB expr RB postfixOp{	//測試test6.java時, 發現有未處理到的文法, 就是array中的迴圈, 故加上了後面兩者
	CheckId($1);
}|prefixOp ID LB expr RB{
	CheckId($2);
}
;
prefixOp: DADD|DSUB|ADD|SUB;
postfixOp:DADD|DSUB;
const_expr: NUM|FNUM|STR;
//處理function call
method_invocation: name LP expr_list RP|name LP RP;
expr_list: expr|expr_list COMMA expr;

//statement3
condition: IF LP bool_expr RP statement else;
else: ELSE statement;
bool_expr: expr infixOp expr;
infixOp:EQ|NEQ|LT|BT|LEQ|BEQ;

//statement4
//loop:
while: WHILE LP bool_expr RP statement|error RC;	//由於有遇到while loop有error時, 沒抓好scope的情形, 所以加上了error RC
for:FOR LP forInit_list SEMI bool_expr SEMI forUpdate RP statement;	//將loop分成兩個處理
forInit_list: forInit|forInit_list COMMA forInit;
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
forUpdate: ID DADD{
	CheckId($1);
}
|ID DSUB{
	CheckId($1);
}
|ID LB expr RB DADD{		//此為處理array再loop裡的問題
	CheckId($1);
}
|ID LB expr RB DSUB{
	CheckId($1);
}
;

//statement5:return
return: RETURN expr SEMI;

%%
int main()
{
	yyparse();
	return 0;
}
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
int LookAll(char *s)	//根據scope layer去確認
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
void CheckId(char *s)		//確認id是否有被宣告過
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
	{
		strcpy(symbolTable[id],s);
		
	}
	else
	{
		if(Look(s))
		{
			strcpy(symbolTable[id],s);
		}
		else
		{
			errtype = 1;
			strcpy(errID1,s);
			id--;
		}
	}
}

