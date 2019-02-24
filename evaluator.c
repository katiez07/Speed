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
		|| strcmp(id, "println") == 0
		|| strcmp(id, "=") == 0)
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
		evalPrint(eval(args, env), env);
	}
	else
		printLex(args);
	return args;
}

Lexeme *evalPlus(Lexeme *tree, Lexeme *env){
	Lexeme *walk = evalArgs(cdr(tree), env);
	if (car(walk)->type == INTEGER){
		int sum = 0;
		while (walk != NULL){
			sum += car(walk)->integer;
			walk = cdr(walk);
		}
		return newIntLexeme(INTEGER, sum);
	}
	else if (car(walk)->type == REAL){
		double sum = 0.0;
		while (walk != NULL){
			sum += car(walk)->real;
			walk = cdr(walk);
		}
		return newRealLexeme(REAL, sum);
	}
	else if (car(walk)->type == FUNCCALL){
		Lexeme *newArgs = evalArgs(walk, env);
		return evalPlus(cons(GLUE, NULL, newArgs), env);
	}
}

Lexeme *evalMinus(Lexeme *tree, Lexeme *env){
	Lexeme *walk = evalArgs(cdr(tree), env);
	if (car(walk)->type == INTEGER){
		int sum = car(walk)->integer;
		walk = cdr(walk);
		while (walk != NULL){
			sum -= car(walk)->integer;
			walk = cdr(walk);
		}
		return newIntLexeme(INTEGER, sum);
	}
	else if (car(walk)->type == REAL){
		double sum = car(walk)->real;
		walk = cdr(walk);
		while (walk != NULL){
			sum -= car(walk)->real;
			walk = cdr(walk);
		}
		return newRealLexeme(REAL, sum);
	}
	else if (car(walk)->type == FUNCCALL){
		Lexeme *newArgs = evalArgs(walk, env);
		return evalMinus(cons(GLUE, NULL, newArgs), env);
	}
}

Lexeme *evalTimes(Lexeme *tree, Lexeme *env){
	Lexeme *walk = evalArgs(cdr(tree), env);
	if (car(walk)->type == INTEGER){
		int sum = car(walk)->integer;
		walk = cdr(walk);
		while (walk != NULL){
			sum *= car(walk)->integer;
			walk = cdr(walk);
		}
		return newIntLexeme(INTEGER, sum);
	}
	else if (car(walk)->type == REAL){
		double sum = car(walk)->real;
		walk = cdr(walk);
		while (walk != NULL){
			sum *= car(walk)->real;
			walk = cdr(walk);
		}
		return newRealLexeme(REAL, sum);
	}
	else if (car(walk)->type == FUNCCALL){
		Lexeme *newArgs = evalArgs(walk, env);
		return evalTimes(cons(GLUE, NULL, newArgs), env);
	}
}

Lexeme *evalDivide(Lexeme *tree, Lexeme *env){
	Lexeme *walk = evalArgs(cdr(tree), env);
	if (car(walk)->type == INTEGER){
		int sum = car(walk)->integer;
		walk = cdr(walk);
		while (walk != NULL){
			sum /= car(walk)->integer;
			walk = cdr(walk);
		}
		return newIntLexeme(INTEGER, sum);
	}
	else if (car(walk)->type == REAL){
		double sum = car(walk)->real;
		walk = cdr(walk);
		while (walk != NULL){
			sum /= car(walk)->real;
			walk = cdr(walk);
		}
		return newRealLexeme(REAL, sum);
	}
	else if (car(walk)->type == FUNCCALL){
		Lexeme *newArgs = evalArgs(walk, env);
		return evalDivide(cons(GLUE, NULL, newArgs), env);
	}
	return NULL;
}

Lexeme *evalAssign(Lexeme *tree, Lexeme *env){
	printEnv(env);
	printLex(car(tree));
	printLex(car(cdr(tree)));
	//return updateVar(env, car(tree), evalArgs(cdr(tree), env));
	return NULL;
}


Lexeme *evalBuiltIn(Lexeme *tree, Lexeme *env){
	char *id = car(tree)->id;
	if (strcmp(id, "print") == 0){
		return evalPrint(car(cdr(tree)), env);
	}
	else if (strcmp(id, "println") == 0)
		printf("\n");
	else if (strcmp(id, "+") == 0)
		return evalPlus(tree, env);
	else if (strcmp(id, "-") == 0)
		return evalMinus(tree, env);
	else if (strcmp(id, "*") == 0)
		return evalTimes(tree, env);
	else if (strcmp(id, "/") == 0)
		return evalDivide(tree, env);
	else if (strcmp(id, "=") == 0){
		return evalAssign(tree, env);
	}
	else 
		printLex(car(tree));

	return NULL;
}



/*
 * more eval functions
 */

Lexeme *evalStructDef(Lexeme *tree, Lexeme *env){

}

Lexeme *evalVarDef(Lexeme *tree, Lexeme *env){
	Lexeme *lnull = newLexeme(NULLVALUE);
       	insertEnv(env, car(tree), lnull);
	return tree;
}

Lexeme *evalFuncDef(Lexeme *tree, Lexeme *env){

}

Lexeme *evalArgs(Lexeme *tree, Lexeme *env){ 
	if (tree == NULL)
		return NULL;
	Lexeme *thing = car(tree);
	Lexeme *l = NULL;
	if (thing->type == INTEGER)
		l = thing;
	else if (thing->type == REAL)
		l = thing;
	else if (thing->type == STRING)
		l = thing;
	else if (thing->type == ID){
		l = getVar(env, thing);
	}
	else if (thing->type == FUNCCALL){
		l = eval(thing, env);
	}

	return cons(ARGS, l, evalArgs(cdr(tree), env));
}

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env){
	if (isBuiltIn(tree))
		return evalBuiltIn(tree, env);
	Lexeme *closure = getVar(env, car(tree));
	Lexeme *args = evalArgs(cdr(tree), env);
	Lexeme *senv = car(closure);
	Lexeme *params = getParams(closure);
	Lexeme *eenv = extendEnv(senv, params, args);
	Lexeme *body = getBody(closure);
	return eval(body, eenv);
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
	else if (tree->type == PROGRAM){
		eval(car(tree), env);
		eval(cdr(tree), env);
		return car(tree);
	}
	else if (tree->type == FUNCCALL)
		return evalFuncCall(tree, env);
	else if (tree->type == VARDEF)
		return evalVarDef(tree, env);
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


