all:	clean y.tab.c lex.yy.c
	gcc lex.yy.c y.tab.c -ly -lfl -o calc

y.tab.c:
	bison -y -d B073040011.y

lex.yy.c:
	flex B073040011.l

clean:
	rm -f calc lex.yy.c y.tab.c y.tab.h 

