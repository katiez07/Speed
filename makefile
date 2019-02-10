
run		:	./environment

environment	:	environment.o lexer.o
			gcc -Wall -Wextra -std=c99 -g -o environment lexer.o environment.o

lexer.o		:	lexer.c lexer.h
			gcc -Wall -Wextra -std=c99 -g -c lexer.c

environment.o	:	lexer.c lexer.h environment.c
			gcc -Wall -Wextra -std=c99 -g -c environment.c

clean		:	
			rm lexer.o environment.o environment

test1		:	

test2		:	

test3		:	

test4		:	

test5		:	

