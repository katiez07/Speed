// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


char pushChar;
int charPushed = 0;
FILE *input;
int lineno = 1;

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
	char ch = read();
	if (ch == '\n')
		lineno++;
	while (isspace(ch)){
		ch = read();
		if (ch == '\n'){
			lineno++;
		}
	}
	pushback(ch);
}


/*
* lexing functions
**/

// constructors
Lexeme *newLexeme(types lexemeType){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->line = lineno;
	return l;
}

Lexeme *newStringLexeme(types lexemeType, char *str){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->line = lineno;
	l->string = str;
	return l;
}

Lexeme *newIntLexeme(types lexemeType, int numInt){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->line = lineno;
	l->integer = numInt;
	return l;
}

Lexeme *newRealLexeme(types lexemeType, double numReal){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->line = lineno;
	l->real = numReal;
	return l;
}

Lexeme *newIDLexeme(types lexemeType, char *id){
	Lexeme *l = malloc(sizeof(Lexeme));
	l->type = lexemeType;
	l->line = lineno;
	l->id = id;
	return l;
}

// lexThing functions
Lexeme *lexSemiOParen(){
	char ch0 = read();
	if (ch0 == '\''){
		return newLexeme(OPAREN);
	}
	else {
		pushback(ch0);
		return newLexeme(SEMI);
	}
}

Lexeme *lexCParen(){
	char ch0 = read();
	if (ch0 == ';'){
		return newLexeme(CPAREN);
	}
	else{
		pushback(ch0);
		return newLexeme(BAD_LEXEME);
	}
}

Lexeme *lexOBrace(){
	char ch0 = read();
	if (ch0 == '/'){
		return newLexeme(OBRACE);
	}
	else{
		pushback(ch0);
		return newLexeme(BAD_LEXEME);
	}
}

Lexeme *lexCommentCBrace(){
	char ch0 = read();
	if (ch0 == '.'){
		return newLexeme(CBRACE);
	}
	else if (ch0 == '/'){
		char ch1 = read();
		while (ch1 != '\n')
			ch1 = read();
		return newLexeme(COMMENT);
	}
	else{
		pushback(ch0);
		return newLexeme(BAD_LEXEME);
	}
}

Lexeme *lexString(){
	char *buff = malloc(sizeof(char));
	char ch0 = read();
	int i = 0;
	while (ch0 != '\"'){
		buff[i] = ch0;
		ch0 = read();
		i++;
	}
	return newStringLexeme(STRING, buff);
}

Lexeme *lexNumber(){
	double real = 0;
	char *buff = malloc(sizeof(char));
	char ch0 = read();
	int i = 0;
	while (!feof(input) && (isdigit(ch0) || ch0 == '.')){
		buff[i] = ch0;
		if (ch0 == '.' && real)
			return newLexeme(BAD_NUMBER);
		if (ch0 == '.')
			real = 1;
		ch0 = read();
		i++;
	}
	pushback(ch0);
	if (real)
		return newRealLexeme(REAL, atof(buff));
	else
		return newIntLexeme(INTEGER, atoi(buff));
}

Lexeme *lexKeyOrID(){
	char *buff = malloc(sizeof(char));
	char ch0 = read();
	int i = 0;
	while (!isspace(ch0) && (ch0 != '/') && (ch0 != '\'')
		&& (ch0 != ';') && (ch0 != '[') && (ch0 != ']')
		&& (ch0 != '.') && (ch0 != '/')){
		buff[i] = ch0;
		ch0 = read();
		i++;
	}
	pushback(ch0);

	if (strcmp(buff, "if") == 0)
		return newLexeme(IF);
	else if (strcmp(buff, "else") == 0)
		return newLexeme(ELSE);
	else if (strcmp(buff, "return") == 0)
		return newLexeme(RETURN);
	else if (strcmp(buff, "struct") == 0)
		return newLexeme(STRUCT);
	else if (strcmp(buff, "func") == 0)
		return newLexeme(FUNCTION);
	else if (strcmp(buff, "var") == 0)
		return newLexeme(VARIABLE);
	else 
		return newIDLexeme(ID, buff);
}

Lexeme *lex(){
	skipWhiteSpace();
	char ch = read();
	if (ch == '[')
		return newLexeme(OBRACKET);
	else if (ch == ']')
		return newLexeme(OBRACKET);
	else if (ch == ';')
		return lexSemiOParen();
	else if (ch == '\'')
		return lexCParen();
	else if (ch == '.')
		return lexOBrace();
	else if (ch == '/')
		return lexCommentCBrace();
	else if (ch == '\"')
		return lexString();
	else if (isdigit(ch)){
		pushback(ch);
		return lexNumber();
	}
	else if (ch >= 33 && ch <= 126){
		pushback(ch);
		return lexKeyOrID();
	}
	else{
		return NULL;
	}

}

