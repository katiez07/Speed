
scanner		:	scanner.o lexer.o
			gcc -Wall -std=c99 -Wextra -g -o scanner lexer.o scanner.o
	
lexer.o		:	lexer.c lexer.h
			gcc -Wall -std=c99 -Wextra -g -c lexer.c

scanner.o	:	lexer.c lexer.h scanner.c
			gcc -Wall -std=c99 -Wextra -g -c scanner.c

clean		:	
			rm lexer.o scanner.o scanner
