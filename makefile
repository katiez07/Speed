

parser		:	parser.o lexer.o
			gcc -Wall -Wextra -std=c99 -g -o parser lexer.o parser.o

lexer.o		:	lexer.c lexer.h
			gcc -Wall -std=c99 -Wextra -g -c lexer.c

parser.o	:	lexer.c lexer.h parser.c
			gcc -Wall -Wextra -std=c99 -g -c parser.c

clean		:	
			rm lexer.o parser.o parser

test1		:	

test2		:	

test3		:	

test4		:	

test5		:	

