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
	clCount = args;
	clArgs = argv;

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

	return 0;
}
