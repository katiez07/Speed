// parser.c
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

Lexeme *program();
Lexeme *expr();
Lexeme *args();
Lexeme *optArgs();
Lexeme *params();
Lexeme *optParams();
Lexeme *ifStatement();
Lexeme *elseStatement();
Lexeme *optElseStatement();
Lexeme *def();
Lexeme *structDef();
Lexeme *varDef();
Lexeme *funcDef();
Lexeme *returnStatement();
Lexeme *unary();
Lexeme *IDexpr();


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
		printf("Fatal error: syntax error on line %d\n", curlex->line);
		printf("illegal\n");

		// extra info on error
		/*
		printf("curlex ");
		printLex(curlex);
		printf("doesn't match expected ");
		printLex(newLexeme(type));
		*/

		exit(-1);
	}
}

Lexeme *match(types type){
	Lexeme *matched;
	matchNoAdvance(type);
	matched = curlex;
	advance();
	return matched;
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

Lexeme *program(){
	Lexeme *e, *p;
	e = expr();
	if (programPending())
		p = program();
	else
		p = NULL;
	return cons(PROGRAM, e, p);
}

Lexeme *expr(){
	if (defPending())
		return def();
	else if (unaryPending())
		return unary();
	else if (ifStatementPending())
		return ifStatement();
	else if (returnStatementPending())
		return returnStatement();
	else if (check(OPAREN)){
		match(OPAREN);
		return expr();
		match(CPAREN);
	}
	else
		return args();
}

Lexeme *optExpr(){
	if (exprPending())
		return expr();
	else
		return NULL;
}

Lexeme *args(){
	Lexeme *u;
	Lexeme *more;
	u = unary();
	if (argsPending())
		more = args();
	else
		more = NULL;
	return cons(ARGS, u, more);
}

Lexeme *optArgs(){
	if (argsPending())
		return args();
	else
		return NULL;
}

Lexeme *params(){
	Lexeme *u;
	Lexeme *more;
	u = match(ID);
	if (paramsPending())
		more = params();
	else
		more = NULL;
	return cons(PARAMS, u, more);
}

Lexeme *optParams(){
	if (paramsPending())
		return params();
	else
		return NULL;
}

Lexeme *ifStatement(){
	Lexeme *cond, *body, *another = NULL;
	match(IF);
	match(OPAREN);
	cond = expr();
	match(CPAREN);
	if (exprPending()){
		body = expr();
		another = optElseStatement();
	}
	else{
		match(OBRACE);
		body = program();
		match(CBRACE);
	}
	return cons(IFELSETOP, cons(IFSTATEMENT, cond, body), another);
}

Lexeme *elseStatement(){
	Lexeme *cond, *body, *another = NULL;
	match(ELSE);
	if (ifStatementPending()){
		return ifStatement();
	}
	else if (exprPending())
		body = expr();
	else{
		match(OBRACE);
		body = program();
		match(CBRACE);
	}
	cond = NULL;
	another = NULL;
	return cons(IFELSETOP, cons(ELSESTATEMENT, cond, body), another);
}

Lexeme *optElseStatement(){
	if (elseStatementPending())
		return elseStatement();
	return NULL;
}

Lexeme *def(){
	if (structDefPending())
		return structDef();
	else if (varDefPending())
		return varDef();
	else // (funcDefPending())
		return funcDef();
}

Lexeme *structDef(){
	Lexeme *id, *p;
	match(STRUCT);
	id = match(ID);
	match(OBRACKET);
	p = program();
	match(CBRACKET);
	return cons(STRUCTDEF, id, p);
}

Lexeme *varDef(){
	Lexeme *id;
	match(VARIABLE);
	id = match(ID);
	match(SEMI);
	return cons(VARDEF, id, NULL);
}

Lexeme *funcDef(){
	Lexeme *id, *params, *body;
	match(FUNCTION);
	id = match(ID);
	match(OPAREN);
	params = optParams();
	match(CPAREN);
	match(OBRACE);
	body = program();
	match(CBRACE);
	return cons(FUNCDEF, id, cons(GLUE, params, body));
}

Lexeme *returnStatement(){
	Lexeme *e;
	match(RETURN);
	e = expr();
	match(SEMI);
	return cons(RETURN, e, NULL);
}

Lexeme *unary(){
	if (IDexprPending()){
		return IDexpr();
	}
	else if (check(INTEGER)){
		return match(INTEGER);
	}
	else if (check(REAL)){
		return match(REAL);
	}
	else if (check(STRING)){
		return match(STRING);
	}
	else {
		match(OPAREN);
		return expr();
		match(CPAREN);
	}
}

Lexeme *IDexpr(){
	Lexeme *id, *args;
	id = match(ID);
	if (check(OPAREN)){
		match(OPAREN);
		args = optArgs();
		match(CPAREN);
	}
	else
		args = NULL;
	return cons(IDEXPR, id, args);
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

	fclose(input);

	return 0;
}
