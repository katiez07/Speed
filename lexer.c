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
	if (charPushed){
		charPushed = 0;
		return pushChar;
	}
	else{
		return fgetc(input);
	}
}

void pushback(char ch){
	if (charPushed){
		fprintf(stderr, "Fatal error: char has already been pushed\n");
		exit(-1);
	}
	charPushed = 1;
	pushChar = ch;
}

void skipWhiteSpace(){
	char ch;
	while (isspace(ch))
		ch = read();
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

Lexeme *newRealLexeme(types lexemeType, double numReal){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->real = numReal;
	return l;
}

// lexThing functions
Lexeme *lexSemiOParen(){
	char ch0 = read();
	char ch1 = read();
	printf("%c%c", ch0, ch1);
	if (ch1 == '\'')
		return newLexeme(OPAREN);
	else {
		pushback(ch1);
		return newLexeme(SEMI);
	}
}

Lexeme *lexCParen(){
	return newLexeme(CPAREN);
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
	char ch = read();
	printf("in lex: %c\n", ch);
	if (ch == '[')
		return newLexeme(OBRACKET);
	else if (ch == ']')
		return newLexeme(OBRACKET);
	else if (ch == ';')
		return lexSemiOParen();
	else if (ch == '\'')
		return lexCParen();
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


