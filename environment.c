// @author Katie Zucker
// environment module

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

/***
 * helper functions
***/

int varMatch(Lexeme *l, char *varname){
	if (l->type == ID && strcmp(l->id, varname) == 0)
		return 1;
	return 0;
}


/***
 * main environment functions
**/

Lexeme *newEnvironment(){
	return cons(ENV, cons(TAB, null, null), null);
}

Lexeme *insertEnv(Lexeme *env, Lexeme *id, Lexeme *val){
	setCar(car(env), cons(I, id, car(car(env)));
	setCdr(car(env), cons(V, val, cdr(car(env)));
	return env;
}

Lexeme *extendEnv(Lexeme *env, Lexeme *listVars, Lexeme *listVals){
	Lexeme *e = newEnvironment();
	setCdr(e, env);
	setCar(car(env), listVars);
	setCdr(car(env), listVals);
	return e;
}

Lexeme *getVar(Lexeme *env, string id){
	Lexeme *vars;
	Lexeme *vals;
	while (env != NULL){
		vars = car(car(env));
		vals = cdr(car(env));
		while (vars != NULL){
			if (varMatch(car(vars), id))
				return car(vars);
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	printf("Fatal error: no variable of that id defined in a reachable environment");
	exit(-1);
}
