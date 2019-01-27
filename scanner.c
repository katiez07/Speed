// scanner.c
// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void printLex(Lexeme *l){
	if (l->type == OBRACKET)
		printf("%s", "OBRACKET\n");
	else if (l->type == CBRACKET)
		printf("%s", "CBRACKET\n");
	else if (l->type == SEMI)
		printf("%s", "SEMI\n");
	else if (l->type == OPAREN)
		printf("%s", "OPAREN\n");
	else
		printf("x\n");
}

int main(int argc, char **argv){
	input = fopen(argv[1], "r");

	Lexeme *l = malloc(sizeof(Lexeme));
	while (!feof(input)){
		l = lex();
		//printLex(l);
	}
	//printLex(l);

	fclose(input);

	return 0;
}
