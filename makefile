
lexer	:	lexer.o
		gcc -Wall -std=c99 -Wextra -g -o lexer lexer.o

lexer.o	:	lexer.c lexer.h
		gcc -Wall -std=c99 -Wextra -g -c lexer.c
clean	:	
		rm lexer.o
