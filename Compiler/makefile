FILE_lex=	B073040011.l
PROG_lex=	lex.yy.c
all:	
	flex $(FILE_lex)
	gcc $(PROG_lex) -o demo -lfl
clean:
	rm demo $(PROG_lex)
