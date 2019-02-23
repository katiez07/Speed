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
		|| strcmp(id, "print") == 0
		|| strcmp(id, "println") == 0)
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
Lexeme *evalPrint(Lexeme *args, Lexeme *env){
	if (args == NULL)
		return NULL;
	else if (args->type == STRING)
		printf("%s", args->string);
	else if (args->type == INTEGER)
		printf("%d", args->integer);
	else if (args->type == REAL)
		printf("%lf", args->real);
	else if (args->type == FUNCCALL){
		Lexeme *l = eval(args, env);
		printLex(l);
		//Lexeme *l2 = newIntLexeme(INTEGER, 5);
		evalPrint(l, env);
		//evalPrint(l2, env);
	}
	else
		printLex(args);
	return args;
}

Lexeme *evalPlus(Lexeme *tree, Lexeme *env){
	Lexeme *walk = cdr(tree);
	if (car(walk)->type == INTEGER){
		int sum = 0;
		while (walk != NULL){
			sum += car(walk)->integer;
			walk = cdr(walk);
		}
		printf("yes");
		return newIntLexeme(INTEGER, sum);
	}
	else{
		double sum = 0.0;
		while (walk != NULL){
			sum += car(walk)->real;
			walk = cdr(walk);
		}
		return newRealLexeme(REAL, sum);
	}
	return NULL;
}

Lexeme *evalMinus(Lexeme *tree, Lexeme *env){
	Lexeme *walk = cdr(tree);
}

Lexeme *evalBuiltIn(Lexeme *tree, Lexeme *env){
	char *id = car(tree)->id;
	if (strcmp(id, "print") == 0){
		return evalPrint(car(cdr(tree)), env);
	}
	else if (strcmp(id, "println") == 0)
		printf("\n");
	else if (strcmp(id, "+") == 0){
		return evalPlus(tree, env);
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
	return NULL;
}

Lexeme *eval(Lexeme *tree, Lexeme *env){
	/*
	printf("evaling:\n");
	printLex(tree);
	printLex(car(tree));
	printLex(cdr(tree));
	*/

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
	else if (tree->type == FUNCCALL)
		return evalFuncCall(tree, env);
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


