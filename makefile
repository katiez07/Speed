lex.o	:	lexer.c lexer.h
		gcc -Wall -std=c99 -Wextra -g -c lexer.c

lex	:	lexer.o
		gcc -Wall -std=c99 -Wextra -g -o lexer lexer.o
