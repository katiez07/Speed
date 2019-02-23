
eval		:	speed.o evaluator.o environment.o parser.o lexer.o
			gcc -Wall -Wextra -std=c99 -g -o eval speed.o lexer.o parser.o environment.o evaluator.o

lexer.o		:	lexer.c lexer.h
			gcc -Wall -Wextra -std=c99 -g -c lexer.c

parser.o	:	lexer.h parser.c parser.h
			gcc -Wall -Wextra -std=c99 -g -c parser.c

environment.o	:	lexer.h environment.h environment.c
			gcc -Wall -Wextra -std=c99 -g -c environment.c

evaluator.o	:	lexer.h parser.h environment.h environment.c evaluator.h evaluator.c
			gcc -Wall -Wextra -std=c99 -g -c evaluator.c

speed.o		:	lexer.h parser.h environment.h evaluator.h speed.c
			gcc -Wall -Wextra -std=c99 -g -c speed.c

clean		:	
			rm lexer.o parser.o environment.o evaluator.o speed.o eval 

run		:	

test1		:	

test2		:	

test3		:	

