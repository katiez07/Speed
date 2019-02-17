
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

run		:	
			make test1
			make test3
			make test2

test1		:	
			./pp test1.spd > test1.pp.1
			./pp test1.pp.1 > test1.pp.2
			diff test1.pp.1 test1.pp.2

test2		:	
			./pp test2.spd > test2.pp.1
			./pp test2.pp.1 > test2.pp.2
			diff test2.pp.1 test2.pp.2

test3		:	
			./pp test3.spd > test3.pp.1
			./pp test3.pp.1 > test3.pp.2
			diff test3.pp.1 test3.pp.2

