// main for whole programming language, Speed
// @author Katie Zucker

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "environment.h"
#include "evaluator.h"


int main(int args, char **argv){
	if (args < 2){
		printf("fatal error: too few args for eval\n");
		exit(-1);
	}

	input = fopen(argv[1], "r");
	curlex = NULL;
	while (curlex == NULL)
		curlex = lex();
	Lexeme *tree = program();
	eval(tree, newEnv());
	fclose(input);

	//printf("%d\n", eval(tree, newEnv())->integer);
	//printLex(eval(tree, newEnv()));

	/*
	Lexeme *arg1 = newIntLexeme(INTEGER, 2);
	Lexeme *arg2 = newIntLexeme(INTEGER, 3);
	Lexeme *id = newIDLexeme(ID, "+");
	Lexeme *plusfunc = cons(FUNCCALL, id, cons(ARGS, arg1, cons(ARGS, arg2, NULL)));
	//printf("%d\n", eval(plusfunc, newEnv())->integer);
	*/

	return 0;
}
