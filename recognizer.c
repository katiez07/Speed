// recognizer.c
// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int exprPending();
int argsPending();
int paramsPending();
int ifStatementPending();
int elseStatementPending();
int defPending();
int structDefPending();
int varDefPending();
int funcDefPending();
int returnStatementPending();
int unaryPending();
int IDexprPending();

void program();
void expr();
void args();
void optArgs();
void params();
void optParams();
void ifStatement();
void elseStatement();
void optElseStatement();
void def();
void structDef();
void varDef();
void funcDef();
void returnStatement();
void unary();
void IDexpr();


void printLex(Lexeme *l){
	if (l == NULL){
		printf("x\n");
		return;
	}
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
	else if (l->type == STRING)
		printf("STRING \"%s\"\n", l->string);
	else if (l->type == REAL)
		printf("REAL %lf\n", l->real);
	else if (l->type == INTEGER)
		printf("INTEGER %d\n", l->integer);
	else if (l->type == IF)
		printf("IF\n");
	else if (l->type == ELSE)
		printf("ELSE\n");
	else if (l->type == RETURN)
		printf("RETURN\n");
	else if (l->type == STRUCT)
		printf("STRUCT\n");
	else if (l->type == FUNCTION)
		printf("FUNCTION\n");
	else if (l->type == VARIABLE)
		printf("VARIABLE\n");
	else if (l->type == ID)
		printf("ID %s\n", l->id);
	else if (l->type == ENDOFFILE)
		printf("ENDOFFILE\n");
	else if (l->type == BAD_NUMBER){
		printf("Fatal error: BAD_NUMBER lexeme on line %d\n", l->line);
		exit(-1);
	}
	else if (l->type == BAD_LEXEME){
		printf("Fatal error: BAD LEXEME on line %d\n", 
			l->line);
		exit(-1);
	}
	else{
		printf("unknown lexeme\n");
	}
}


/*** helpers ***/

int check(types type){
	return curlex->type == type;
}

void advance(){
	curlex = lex();
}

void matchNoAdvance(types type){
	if (!check(type)){
		printf("Fatal error: syntax error on line %d\n, curlex", curlex->line);
		printLex(curlex);
		printf("doesn't match ");
		printLex(newLexeme(type));
		exit(-1);
	}
}

void match(types type){
	matchNoAdvance(type);
	advance();
}


/*** pending functions ***/

int programPending(){
	return exprPending();
}

int exprPending(){
	if (defPending())
		return 1;
	else if (unaryPending())
		return 1;
	else if (ifStatementPending())
		return 1;
	else if (returnStatementPending())
		return 1;
	else if (check(OPAREN)){
		return check(OPAREN);
	}
	else
		return argsPending();
}

int argsPending(){
	return unaryPending();
}

int paramsPending(){
	return check(ID);
}

int ifStatementPending(){
	return check(IF);
}

int elseStatementPending(){
	return check(ELSE);
}

int defPending(){
	if (structDefPending())
		return 1;
	else if (varDefPending())
		return 1;
	else 
		return funcDefPending();
}

int structDefPending(){
	return check(STRUCT);
}

int varDefPending(){
	return check(VARIABLE);
} 

int funcDefPending(){
	return check(FUNCTION);
}

int returnStatementPending(){
	return check(RETURN);
}

int unaryPending(){
	if (IDexprPending())
		return 1;
	else if (check(INTEGER))
		return 1;
	else if (check(REAL))
		return 1;
	else if (check(STRING))
		return 1;
	else if (check(OPAREN))
		return 1;
	return 0;
}

int IDexprPending(){
	return check(ID);
}


/*** calling functions ***/

void program(){
	expr();
	if (programPending())
		program();
}

void expr(){
	if (defPending())
		def();
	else if (unaryPending())
		unary();
	else if (ifStatementPending())
		ifStatement();
	else if (returnStatementPending())
		returnStatement();
	else if (check(OPAREN)){
		match(OPAREN);
		expr();
		match(CPAREN);
	}
	else
		args();
}

void optExpr(){
	if (exprPending())
		expr();
}

void args(){
	unary();
	if (argsPending())
		args();
}

void optArgs(){
	if (argsPending())
		args();
}

void params(){
	match(ID);
	if (paramsPending())
		params();
}

void optParams(){
	if (paramsPending())
		params();
}

void ifStatement(){
	match(IF);
	match(OPAREN);
	expr();
	match(CPAREN);
	if (exprPending()){
		expr();
		optElseStatement();
	}
	else{
		match(OBRACE);
		program();
		match(CBRACE);
	}
}

void elseStatement(){
	match(ELSE);
	if (ifStatementPending())
		ifStatement();
	else if (exprPending())
		expr();
	else{
		match(OBRACE);
		program();
		match(CBRACE);
	}
}

void optElseStatement(){
	if (elseStatementPending())
		elseStatement();
}

void def(){
	if (structDefPending())
		structDef();
	else if (varDefPending())
		varDef();
	else if (funcDefPending())
		funcDef();
}

void structDef(){
	match(STRUCT);
	unary();
	match(OBRACKET);
	program();
	match(CBRACKET);
}

void varDef(){
	match(VARIABLE);
	match(ID);
	if (exprPending()){
		expr();
		match(SEMI);
	}
	else
		match(SEMI);
}

void funcDef(){
	match(FUNCTION);
	match(ID);
	match(OPAREN);
	optParams();
	match(CPAREN);
	match(OBRACE);
	program();
	match(CBRACE);
}

void returnStatement(){
	match(RETURN);
	expr();
	match(SEMI);
}

void unary(){
	if (IDexprPending()){
		IDexpr();
	}
	else if (check(INTEGER)){
		match(INTEGER);
	}
	else if (check(REAL)){
		match(REAL);
	}
	else if (check(STRING)){
		match(STRING);
	}
	else {
		match(OPAREN);
		expr();
		match(CPAREN);
	}
}

void IDexpr(){
	match(ID);
	if (check(OPAREN)){
		match(OPAREN);
		optArgs();
		match(CPAREN);
	}
}

void testIDexpr(){
	match(ID);
}


/*** main ***/

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Fatal error: too few args\n");
		exit(-1);
	}

	input = fopen(argv[1], "r");

	curlex = NULL;
	while (curlex == NULL){
		curlex = lex();
	}

	program();

	/*
	while (!feof(input)){
		skipWhiteSpace();
		printLex(curlex);
		curlex = lex();
	}
	*/

	fclose(input);

	return 0;
}
