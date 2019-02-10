
environment	:	testEnv.o environment.o lexer.o
			gcc -Wall -Wextra -std=c99 -g -o environment lexer.o environment.o testEnv.o

run		:	
			./environment

lexer.o		:	lexer.c lexer.h
			gcc -Wall -Wextra -std=c99 -g -c lexer.c

environment.o	:	lexer.c lexer.h environment.c environment.h
			gcc -Wall -Wextra -std=c99 -g -c environment.c

testEnv.o	:	lexer.c lexer.h environment.c environment.h testEnv.c
			gcc -Wall -Wextra -std=c99 -g -c testEnv.c

clean		:	
			rm lexer.o environment.o environment testEnv.o
