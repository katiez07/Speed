// @author Katie Zucker
// test for envivronment module

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "environment.h"

int main(){
	printf("creating new environment\n");
	Lexeme *env0 = newEnv();
	printf("the environment is:\n");
	printEnv(env0);
	printf("adding var x=4\n");
	Lexeme *xvar = newIDLexeme(ID, "x");
	Lexeme *xval = newIntLexeme(INTEGER, 4);
	insertEnv(env0, xvar, xval);
	printf("now the environment is:\n");
	printEnv(env0);
	printf("adding var y=2\n");
	Lexeme *yvar = newIDLexeme(ID, "y");
	Lexeme *yval = newIntLexeme(INTEGER, 2);
	insertEnv(env0, yvar, yval);
	printf("now the environment is:\n");
	printEnv(env0);
	printf("extending the environment with str=\"hello Speed!!\" and z=6.7\n");
	Lexeme *strvar = newIDLexeme(ID, "str");
	Lexeme *strval = newStringLexeme(STRING, "hello Speed!!");
	Lexeme *zvar = newIDLexeme(ID, "z");
	Lexeme *zval = newRealLexeme(REAL, 6.7);
	Lexeme *vars = cons(I, zvar, cons(I, strvar, NULL));
	Lexeme *vals = cons(V, zval, cons(I, strval, NULL));
	Lexeme *e1 = extendEnv(env0, vars, vals);
	printf("now the environment is extended! The local environment is:\n");
	printLocalEnv(e1);
	printf("the entire environment is:\n");
	printEnv(e1);
	printf("extending the environment with x=12\n");
	Lexeme *x2var = newIDLexeme(ID, "x");
	Lexeme *x2val = newIntLexeme(INTEGER, 12);
	Lexeme *e2 = extendEnv(e1, cons(I, x2var, NULL), cons(V, x2val, NULL));
	printf("the local environment is:\n");
	printLocalEnv(e2);
	printf("the entire environment is:\n");
	printEnv(e2);
	printf("looking up value for x\n");
	Lexeme *xex = newIDLexeme(ID, "x");
	Lexeme *xreturned = getVar(e2, xex);
	printf("Found value for x is %d\n", xreturned->integer);
	printf("setting x=13 in the nearest scope\n");
	Lexeme *x2val2 = newIntLexeme(INTEGER, 13);
	updateVar(e2, xex, x2val2);
	printf("now the entire environment is:\n");
	printEnv(e2);
	printf("setting z=6.4\n");
	Lexeme *zvar2 = newIDLexeme(ID, "z");
	Lexeme *zval2 = newRealLexeme(REAL, 6.4);
	updateVar(e2, zvar2, zval2);
	printf("now the entire environment is:\n");
	printEnv(e2);

	return 0;	
}
