// @author Katie Zucker
// file for testing prettyprint

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv){
	if (argc < 2){
		printf("Fatal error: too few args\n");
	}

	input = fopen(argv[1], "r");

	curlex = NULL;
	while (curlex == NULL){
		curlex = lex();
	}

	Lexeme *tree = program();
	prettyprint(tree, stdout);

	fclose(input);

	return 0;
}
