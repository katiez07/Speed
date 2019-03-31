// @author Katie Zucker
// environment module

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "environment.h"

/***
 * helper functions
***/

int varMatch(Lexeme *a, Lexeme *b){
	if (a->type == ID && strcmp(a->id, b->id) == 0)
		return 1;
	return 0;
}

void printVal(Lexeme *val){
	if (val == NULL)
		return;
	else if (val->type == STRING)
		printf("%s", val->string);
	else if (val->type == INTEGER)
		printf("%d", val->integer);
	else if (val->type == REAL)
		printf("%lf", val->real);
	else if (val->type == NULLVALUE)
		printf("NULLVALUE");
}


/***
 * main environment functions
**/

Lexeme *newEnv(){
	return cons(ENV, cons(TAB, NULL, NULL), NULL);
}

Lexeme *insertEnv(Lexeme *env, Lexeme *id, Lexeme *val){
	setCar(car(env), cons(I, id, car(car(env))));
	setCdr(car(env), cons(V, val, cdr(car(env))));
	return val;
}

Lexeme *extendEnv(Lexeme *env, Lexeme *listVars, Lexeme *listVals){
	Lexeme *e = newEnv();
	setCdr(e, env);
	setCar(car(e), listVars);
	setCdr(car(e), listVals);
	return e;
}

Lexeme *getVar(Lexeme *env, Lexeme *var){
	Lexeme *vars;
	Lexeme *vals;
	while (env != NULL){
		vars = car(car(env));
		vals = cdr(car(env));
		while (vars != NULL){
			if (varMatch(car(vars), var))
				return car(vals);
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	printf("Fatal error: no variable of that id defined in a reachable environment\n");
	exit(-1);
}

Lexeme *updateVar(Lexeme *env, Lexeme *var, Lexeme *val){
	Lexeme *vars;
	Lexeme *vals;
	Lexeme *oldVal;
	while (env != NULL){
		vars = car(car(env));
		vals = cdr(car(env));
		while (vars != NULL){
			if (varMatch(car(vars), var)){
				oldVal = car(vals);
				setCar(vals, val);
				return oldVal;
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	printf("Fatal error: could not update value because no variable of that id defined in a reachable environment");
	exit(-1);
}

void printLocalEnv(Lexeme *env){
	Lexeme *vars = car(car(env));
	Lexeme *vals = cdr(car(env));
	printf("Local env\n---------\n");
		while (vars != NULL){
		printf("%s:", car(vars)->id);
		printVal(car(vals));
		printf("\n");
		vars = cdr(vars);
		vals = cdr(vals);
	}
	printf("\n");
}

void printEnv(Lexeme *env){
	Lexeme *vars;
	Lexeme *vals;
	int envNo = 0;
	while (env != NULL){
		printf("Env %d\n-----\n", envNo);
		vars = car(car(env));
		vals = cdr(car(env));
		while (vars != NULL){
			printf("%s:", car(vars)->id);
			printVal(car(vals));
			printf("\n");
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
		envNo++;
		printf("\n");
	}
}

