
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
			cat error4.spd
			-./eval error4.spd
			cat error5.spd
			-./eval error5.spd
			cat arrays.spd
			-./eval arrays.spd

problem		:	
			cat testfilesumnums.spd

problemx	:
			-./eval testfilesumnums.spd

error1		:	
			cat error1.spd

error1x		:	
			-./eval error1.spd

error2		:	
			cat error2.spd

error2x		:	
			-./eval error2.spd

error3		:	
			cat error3.spd

error3x		:
			-./eval error3.spd

error4		:	
			cat error4.spd

error4x		:	
			-./eval error4.spd

error5		:	
			cat error5.spd

error5x 	:	
			-./eval error5.spd

arrays		:	
			cat testarrays.spd

arraysx		:
			-./eval testarrays.spd

conditionals	:	
			cat testifelse2.spd

conditionalsx	:	
			-./eval testifelse2.spd

recursion	:	
			cat testrecur.spd

recursionx	:	
			-./eval testrecur.spd

iteration	:	
			cat testiter.spd

iterationx	:	
			-./eval testiter.spd

functions	:	
			cat testfunctions2.spd

functionsx	:	
			-./eval testfunctions2.spd

lambda		:
			cat testllama2.spd

lambdax		:
			-./eval testllama2.spd

objects		:
			cat testobj.spd

objectsx	:
			-./eval testobj.spd


