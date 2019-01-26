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
	return (char)fgetc(input);
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
Lexeme *newLexeme(types lexemeType){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	return l;
}

Lexeme *newStringLexeme(types lexemeType, char *str){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->string = str;
	return l;
}

Lexeme *newIntLexeme(types lexemeType, int numInt){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->integer = numInt;
	return l;
}

// lexThing functions
Lexeme *lexSemiParen(){
	Lexeme *thing = malloc(sizeof(Lexeme));

	return newLexeme(SEMI);
}

Lexeme *lexNumber(){
	return NULL;
}

Lexeme *lexVarOrKey(){
	return NULL;
}

Lexeme *lexString(){
	return NULL;
}

Lexeme *lex(){
	Lexeme *thing = malloc(sizeof(Lexeme));
	
	skipWhiteSpace();
	//char ch = read();
	char ch = read();
	if (ch == '[')
		return newLexeme(OBRACKET);
	else if (ch == ']')
		return newLexeme(OBRACKET);
	else if (ch == ';')
		return lexSemiParen();
	else if (isdigit(ch))
		return lexNumber();
	else if (ch == '\"')
		return lexString();
	else if (isalpha(ch))
		return lexVarOrKey();
	else
		return newLexeme(BAD_LEXEME);

	return NULL;
}


