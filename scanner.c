// scanner.c
// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void printLex(Lexeme *l){
	if (l == NULL)
		return;
	if (l->type == OBRACKET)
		printf("OBRACKET\n");
	else if (l->type == CBRACKET)
		printf("CBRACKET\n");
	else if (l->type == SEMI)
		printf("SEMI\n");
	else if (l->type == OPAREN)
		printf("OPAREN\n");
	else if (l->type == CPAREN)
		printf("CPAREN\n");
	else if (l->type == OBRACE)
		printf("OBRACE\n");
	else if (l->type == CBRACE)
		printf("CBRACE\n");
	else if (l->type == COMMENT)
		printf("COMMENT\n");
	else if (l->type == STRING)
		printf("STRING\n");
	else if (l->type == REAL)
		printf("REAL %lf\n", l->real);
	else if (l->type == INTEGER)
		printf("INTEGER %d\n", l->integer);
	else if (l->type == BAD_LEXEME){
		fprintf(stderr, "BAD LEXEME\n");
		//exit(-1);
	}
		
	else
		fprintf(stderr, "Lexeme doesn't have a type\n");
}

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Fatal error: too few args");
		exit(-1);
	}

	input = fopen(argv[1], "r");

	while (!feof(input)){
		skipWhiteSpace();
		printLex(lex());
	}
	//printLex(l);

	fclose(input);

	return 0;
}
