
recognizer	:	recognizer.o lexer.o
			gcc -Wall -Wextra -std=c99 -g -o recognizer lexer.o recognizer.o

lexer.o		:	lexer.c lexer.h
			gcc -Wall -std=c99 -Wextra -g -c lexer.c

recognizer.o	:	lexer.c lexer.h recognizer.c
			gcc -Wall -Wextra -std=c99 -g -c recognizer.c

clean		:	
			rm lexer.o recognizer.o recognizer

test1		:	
			recognizer program.txt

test2		:	
			recognizer prog1.txt

test3		:	
			recognizer prog2.txt

test4		:	
			recognizer prog3.txt

test5		:	
			recognizer prog4.txt

run		:	
			recognizer program.txt
