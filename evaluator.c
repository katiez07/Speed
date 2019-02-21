// eval
// @author Katie Zucker

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "environment.h"
#include "evaluator.h"


/*
 * helper functions
 */

int isBuiltIn(Lexeme *f){
	char *id = car(f)->id;
	if (strcmp(id, "+") == 0
		|| strcmp(id, "-") == 0
		|| strcmp(id, "*") == 0
		|| strcmp(id, "/") == 0
		|| strcmp(id, "print") == 0)
		return 1;
	return 0;
	return 1;
}

Lexeme *getParams(Lexeme *f){
	return car(cdr(f));
}

Lexeme *getBody(Lexeme *f){
	return cdr(cdr(f));
}


/*
 * eval Built-Ins
 */

Lexeme *evalBuiltIn(Lexeme *tree, Lexeme *env){
	char *id = car(tree)->id;
	if (strcmp(id, "print") == 0){
		printf("%s\n", car(cdr(tree))->string);
		return car(cdr(tree));
	}
	else if (strcmp(id, "+") == 0){
		Lexeme *walk = tree;
		if (car(cdr(tree))->type == INTEGER){
			int sum = 0;
			while (walk != NULL){
				sum += car(cdr(tree))->integer;
				walk = cdr(walk);
			}
			return newIntLexeme(INTEGER, sum);
		}
		else{
			double sum = 0;
			while (walk != NULL){
				sum += car(cdr(walk))->real;
				walk = cdr(walk);
			}
			return newRealLexeme(REAL, sum);
		}
	}

	return NULL;
}



/*
 * more eval functions
 */

Lexeme *evalVarDef(Lexeme *env, Lexeme *var){
	Lexeme *lnull = newLexeme(NULLVALUE);
	insertEnv(env, car(var), lnull);
}

Lexeme *evalArgs(Lexeme *tree, Lexeme *env){

}

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env){
	// temp strcmp version
	if (isBuiltIn(tree))
		return evalBuiltIn(tree, env);
	/*
	Lexeme *closure = getVar(env, car(tree));
	Lexeme *args = evalArgs(cdr(tree), env);
	//if (isBuiltIn(closure))
	//	return evalBuiltIn(tree, closure);
	Lexeme *senv = car(closure);
	Lexeme *params = getParams(closure);
	Lexeme *eenv = extendEnv(senv, params, args);
	Lexeme *body = getBody(closure);
	return eval(body, eenv);
	*/
}

Lexeme *eval(Lexeme *tree, Lexeme *env){
	if (tree == NULL)
		return NULL;
/*
	else if (tree->type == ID)
		return tree->id;
	else if (tree->type == INTEGER)
		return tree->integer;
	else if (tree->type == REAL)
		return tree->real;
	else if (tree->type == STRING)
		return tree->string;
	else if (tree->type == NULLVALUE)
		return NULL;
*/
	else if (tree->type == PROGRAM){
		eval(car(tree), env);
		eval(cdr(tree), env);
		return car(tree);
	}
	else if (tree->type == FUNCCALL){
		return evalFuncCall(tree, env);
	}
	else{
		//printLex(tree);
		return NULL;
	}
	/*
	else if (tree->type == VARDEF)
		return evalVarDef(tree, env);
	else if (tree->type == FUNCCALL)
		return evalFuncCall(tree, env);
	else{
		printf("?\n");
		return NULL;
	}
	// ...
	*/
}


