// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


char pushChar;
int charPushed = 0;
FILE *input;

/*
* helper functions
*/

char read(){
	return 'x';
	//return (char)fgetc(input);
}

char pushback(ch){
	if (charPushed){
		charPushed = 0;
		return (char)pushChar;
	}
	return (char)read();
}

void skipWhiteSpace(){
	char ch;
	while (isspace(ch))
		ch = read();

	pushback(ch);
}


/*
* lexing functions
**/

// constructors
Lexeme *newLexeme(lexemeType){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	return l;
}

// lexThing functions
Lexeme *lexSemiParen(){
	Lexeme *thing = malloc(sizeof(Lexeme));
	return NULL;
}

Lexeme *lex(){
	Lexeme *thing = malloc(sizeof(Lexeme));
	
	skipWhiteSpace();
	//char ch = read();
	char ch = 'x';
	if (ch == '[')
		return newLexeme(OBRACKET);
	else if (ch == ']')
		return newLexeme(OBRACKET);
	else if (ch == ';')
		return lexSemiParen();


	return thing;
}


int main (int argc, char **argv){
	input = fopen(argv[1], "r");
	
	while (!feof(input)){
		printf("%c", read());
	}
	
	fclose(input);
	
	return 0;
}
