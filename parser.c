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
int arrDefPending();

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
Lexeme *arrDef();


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
	else if (l->type == PROGRAM)
		printf("PROGRAM\n");
	else if (l->type == EXPR)
		printf("EXPR\n");
	else if (l->type == UNARY)
		printf("UNARY\n");
	else if (l->type == IDEXPR)
		printf("IDEXPR\n");
	else if (l->type == ARGS)
		printf("ARGS\n");
	else if (l->type == PARAMS)
		printf("PARAMS\n");
	else if (l->type == IFELSETOP)
		printf("IFELSETOP\n");
	else if (l->type == IFSTATEMENT)
		printf("IFSTATEMENT\n");
	else if (l->type == ELSESTATEMENT)
		printf("ELSESTATEMENT\n");
	else if (l->type == DEF)
		printf("DEF\n");
	else if (l->type == STRUCTDEF)
		printf("STRUCTDEF\n");
	else if (l->type == VARDEF)
		printf("VARDEF\n");
	else if (l->type == FUNCDEF)
		printf("FUNCDEF\n");
	else if (l->type == RETURNSTATEMENT)
		printf("RETURNSTATEMENT\n");
	else if (l->type == FUNCCALL)
		printf("FUNCCALL\n");
	else if (l->type == ARGS)
		printf("ARGS\n");
	else if (l->type == GLUE)
		printf("GLUE\n");
	else if (l->type == ENV)
		printf("ENV %p\n", l);
	else if (l->type == TAB)
		printf("TAB\n");
	else if (l->type == I)
		printf("I\n");
	else if (l->type == V)
		printf("I\n");
	else if (l->type == NULLVALUE)
		printf("NULLVALUE\n");
	else if (l->type == CLOSURE)
		printf("CLOSURE\n");
	else if (l->type == ARRAY)
		printf("ARRAY\n");
	else if (l->type == ARRDEF)
		printf("ARRDEF\n");
	else if (l->type == ARRCALL)
		printf("ARRCALL\n");
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
		printf("curlex ");
		printLex(curlex);
		printf("doesn't match expected ");
		printLex(newLexeme(type));

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
	else if (arrDefPending())
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

int arrDefPending(){
	return check(ARRAY);
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
	else if (IDexprPending())
		return IDexpr();
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
	}
	else{
		match(OBRACE);
		body = program();
		match(CBRACE);
	}
	another = optElseStatement();
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
	else if (arrDefPending())
		return arrDef();
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

Lexeme *arrDef(){
	Lexeme *id, *vals;
	match(ARRAY);
	id = match(ID);
	match(OBRACKET);
	vals = optArgs();
	match(CBRACKET);
	match(SEMI);
	return cons(ARRDEF, id, vals);
}

Lexeme *returnStatement(){
	Lexeme *e;
	match(RETURN);
	e = expr();
	match(SEMI);
	return cons(RETURNSTATEMENT, e, NULL);
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
		return cons(FUNCCALL, id, args);
	}
	else if (check(OBRACKET)){
		match(OBRACKET);
		args = match(INTEGER);
		match(CBRACKET);
		return cons(ARRCALL, id, args);
	}
	else
		return id;
}

// prettyprint

void prettyprint(Lexeme *tree){
	if (tree == NULL)
		return;
	else if (tree->type == ID)
		printf("%s ", tree->id);
	else if (tree->type == INTEGER)
		printf("%d ", tree->integer);
	else if (tree->type == REAL)
		printf("%lf ", tree->real);
	else if (tree->type == STRING)
		printf("\"%s\" ", tree->string);
	else if (tree->type == FUNCCALL){
		printf("%s ;' ", car(tree)->id);
		prettyprint(cdr(tree));
		printf(" ';\n");
	}
	else if (tree->type == STRUCTDEF){
		printf("struct ");
		prettyprint(car(tree));
		printf("\n[\n");
		prettyprint(cdr(tree));
		printf("]\n\n");
	}
	else if (tree->type == VARDEF){
		printf("var ");
		prettyprint(car(tree));
		printf(";\n");
	}
	else if (tree->type == FUNCDEF){
		printf("func ");
		prettyprint(car(tree));
		printf(";' ");
		prettyprint(car(cdr(tree)));
		printf(" '; \n./\n");
		prettyprint(cdr(cdr(tree)));
		printf("/.\n\n");
	}
	else if (tree->type == PARAMS){
		printf(" ");
		while (tree != NULL){
			prettyprint(car(tree));
			tree = cdr(tree);
			printf(" ");
		}
	}
	else if (tree->type == ARGS){
		printf(" ");
		while (tree != NULL){
			prettyprint(car(tree));
			tree = cdr(tree);
			printf(" ");
		}
	}
	else if (tree->type == IFELSETOP){
		// this first part takes care of top if
		printf("if ;' ");
			prettyprint(car(car(tree)));
		printf(" ';\n./\n");
		prettyprint(cdr(car(tree)));
		printf("/.\n");
		tree = cdr(tree);
		while (tree != NULL){
			// this part takes care of all else ifs and else
			prettyprint(car(tree));
			tree = cdr(tree);
		}
	}
	else if (tree->type == IFSTATEMENT){
		printf("else if ;' ");
		prettyprint(car(tree));
		printf(" '; \n./\n");
		prettyprint(cdr(tree));
		printf("/.\n");
	}
	else if (tree->type == ELSESTATEMENT){
		printf("else \n./\n");
		prettyprint(cdr(tree));
		printf("/.\n");
	}
	else if (tree->type == RETURNSTATEMENT){
		printf("return ");
		prettyprint(car(tree));
		printf(";\n");
	}
	else if (tree->type == PROGRAM){
		while (tree != NULL){
			prettyprint(car(tree));
			tree = cdr(tree);
		}
	}
	else{
		printLex(tree);
	}
}
