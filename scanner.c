// scanner.c
// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void printLex(Lexeme *l){
	if (l == NULL)
		return;
	if (l->type == OBRACKET)
		printf("%s", "OBRACKET\n");
	else if (l->type == CBRACKET)
		printf("%s", "CBRACKET\n");
	else if (l->type == SEMI)
		printf("%s", "SEMI\n");
	else if (l->type == OPAREN)
		printf("%s", "OPAREN\n");
	else if (l->type == CPAREN)
		printf("%s", "CPAREN\n");
	else if (l->type == OBRACE)
		printf("%s", "OBRACE\n");
	else if (l->type == CBRACE)
		printf("%s", "CBRACE\n");
	else if (l->type == STRING)
		printf("%s", "STRING\n");
	else if (l->type == BAD_LEXEME){
		fprintf(stderr, "BAD LEXEME\n");
		//exit(-1);
	}
		
	else
		fprintf(stderr, "Lexeme doesn't have a type\n");
}

int main(int argc, char **argv){
	input = fopen(argv[1], "r");

	Lexeme *l = malloc(sizeof(Lexeme));
	while (!feof(input)){
		l = lex();
		printLex(l);
	}
	//printLex(l);

	fclose(input);

	return 0;
}
