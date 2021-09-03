/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BOOLEAN = 258,
    BREAK = 259,
    BYTE = 260,
    CASE = 261,
    CHAR = 262,
    CATCH = 263,
    CLASS = 264,
    CONST = 265,
    CONTINUE = 266,
    DEFAULT = 267,
    DO = 268,
    DOUBLE = 269,
    ELSE = 270,
    EXTENDS = 271,
    FALSE = 272,
    FINAL = 273,
    FINALLY = 274,
    FLOAT = 275,
    FOR = 276,
    IF = 277,
    IMPLEMENTS = 278,
    INT = 279,
    LONG = 280,
    MAIN = 281,
    NEW = 282,
    PRINT = 283,
    PRIVATE = 284,
    PROTECTED = 285,
    PUBLIC = 286,
    RETURN = 287,
    READ = 288,
    SHORT = 289,
    STATIC = 290,
    STRING = 291,
    SWITCH = 292,
    THIS = 293,
    TRUE = 294,
    TRY = 295,
    VOID = 296,
    WHILE = 297,
    COMMA = 298,
    COLON = 299,
    SEMI = 300,
    LP = 301,
    RP = 302,
    LC = 303,
    RC = 304,
    LB = 305,
    RB = 306,
    DOT = 307,
    DADD = 308,
    DSUB = 309,
    EQ = 310,
    BEQ = 311,
    LEQ = 312,
    NEQ = 313,
    AND = 314,
    OR = 315,
    NOT = 316,
    ADD = 317,
    SUB = 318,
    MUL = 319,
    DIV = 320,
    MOD = 321,
    ASSIGN = 322,
    LT = 323,
    BT = 324,
    FNUM = 325,
    STR = 326,
    NUM = 327,
    CH = 328,
    ID = 329
  };
#endif
/* Tokens.  */
#define BOOLEAN 258
#define BREAK 259
#define BYTE 260
#define CASE 261
#define CHAR 262
#define CATCH 263
#define CLASS 264
#define CONST 265
#define CONTINUE 266
#define DEFAULT 267
#define DO 268
#define DOUBLE 269
#define ELSE 270
#define EXTENDS 271
#define FALSE 272
#define FINAL 273
#define FINALLY 274
#define FLOAT 275
#define FOR 276
#define IF 277
#define IMPLEMENTS 278
#define INT 279
#define LONG 280
#define MAIN 281
#define NEW 282
#define PRINT 283
#define PRIVATE 284
#define PROTECTED 285
#define PUBLIC 286
#define RETURN 287
#define READ 288
#define SHORT 289
#define STATIC 290
#define STRING 291
#define SWITCH 292
#define THIS 293
#define TRUE 294
#define TRY 295
#define VOID 296
#define WHILE 297
#define COMMA 298
#define COLON 299
#define SEMI 300
#define LP 301
#define RP 302
#define LC 303
#define RC 304
#define LB 305
#define RB 306
#define DOT 307
#define DADD 308
#define DSUB 309
#define EQ 310
#define BEQ 311
#define LEQ 312
#define NEQ 313
#define AND 314
#define OR 315
#define NOT 316
#define ADD 317
#define SUB 318
#define MUL 319
#define DIV 320
#define MOD 321
#define ASSIGN 322
#define LT 323
#define BT 324
#define FNUM 325
#define STR 326
#define NUM 327
#define CH 328
#define ID 329

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 34 "B073040011.y" /* yacc.c:1909  */

char* strval;

#line 206 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
