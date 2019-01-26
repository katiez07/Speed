// scanner.c
// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void printLex(Lexeme *l){
	if (l->type == OBRACKET)
		printf("%s", "OBRACKET");
}

int main(int argc, char **argv){
	input = fopen(argv[1], "r");

	while (!feof(input)){
		lex();
	}

	fclose(input);

	return 0;
}
