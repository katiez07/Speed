run		:	pp program.txt

pp		:	testpp.o parser.o lexer.o
			gcc -Wall -Wextra -std=c99 -g -o pp lexer.o parser.o testpp.o

lexer.o		:	lexer.c lexer.h
			gcc -Wall -Wextra -std=c99 -g -c lexer.c

parser.o	:	lexer.h parser.c parser.h
			gcc -Wall -Wextra -std=c99 -g -c parser.c

testpp.o	:	lexer.h parser.h testpp.c 
			gcc -Wall -Wextra -std=c99 -g -c testpp.c

clean		:	
			rm lexer.o parser.o testpp.o pp

test1		:	

test2		:	

test3		:	

test4		:	

test5		:	

