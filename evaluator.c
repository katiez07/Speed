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
	else if (args->type == NULLVALUE)
		printf("NULL");
	else if (args->type == ID)
		evalPrint(evalID(args, env), env);
	else if (args->type == FUNCCALL){
		//printLex(car(args));
		//printLex(eval(args, env));
		evalPrint(eval(args, env), env);
	}
	else if (args->type == RETURNSTATEMENT){
		evalPrint(eval(args, env), env);
	}
	//else
		//printLex(args);
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
	return NULL;
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
	return NULL;
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
	return NULL;
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
	if (tree == NULL)
		return NULL;
	if (cdr(cdr(cdr(tree))) != NULL){
		printf("fatal error near line %d: too many args to assign\n", car(tree)->line);
		exit(-1);
	}
	updateVar(env, car(cdr(tree)), car(evalArgs(cdr(cdr(tree)), env)));
	return tree;
}


Lexeme *evalBuiltIn(Lexeme *tree, Lexeme *env){
	char *id = car(tree)->id;
	if (strcmp(id, "print") == 0)
		return evalPrint(car(cdr(tree)), env);
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
	else if (strcmp(id, "=") == 0)
		return evalAssign(tree, env);
	else 
		printLex(car(tree));

	return NULL;
}



/*
 * more eval functions
 */

/*
Lexeme *evalStructDef(Lexeme *tree, Lexeme *env){

}
*/

Lexeme *evalVarDef(Lexeme *tree, Lexeme *env){
	Lexeme *lnull = newLexeme(NULLVALUE);
       	insertEnv(env, car(tree), lnull);
	return tree;
}

Lexeme *evalFuncDef(Lexeme *tree, Lexeme *env){
	Lexeme *clos = cons(CLOSURE, env, tree);
	insertEnv(env, car(tree), clos);
	//printLex(cdr(cdr(cdr(clos))));
	//printLex(car(cdr(cdr(cdr(clos)))));
	//printLex(car(car(cdr(cdr(cdr(clos))))));
	//printLex(cdr(cdr(cdr(cdr(clos)))));
	//printLex(car(cdr(cdr(cdr(cdr(clos))))));
	//printLex(cdr(cdr(cdr(cdr(cdr(clos))))));
	return clos;
}

Lexeme *evalArrDef(Lexeme *tree, Lexeme *env){
	insertEnv(env, car(tree), tree);
	return env;
}

Lexeme *evalArrCall(Lexeme *tree, Lexeme *env){
	if (cdr(tree)->type != INTEGER){
		printf("fatal error on line %d: array index not an integer\n", cdr(tree)->line);
		exit(-1);
	}
	Lexeme *arrdef = getVar(env, car(tree));
	Lexeme *val = cdr(arrdef);
	int index = cdr(tree)->integer;
	int i = 0;
	while (i < index)
		val = cdr(val);
	return val;
}

Lexeme *evalArgs(Lexeme *tree, Lexeme *env){ 
	if (tree == NULL || car(tree) == NULL)
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
		l = evalID(thing, env);
	}
	else if (thing->type == FUNCCALL){
		l = eval(thing, env);
	}

	return cons(ARGS, l, evalArgs(cdr(tree), env));
}

Lexeme *evalID(Lexeme *tree, Lexeme *env){
	return getVar(env, tree);
}

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env){
	if (isBuiltIn(tree))
		return evalBuiltIn(tree, env);
	Lexeme *closure = getVar(env, car(tree));
	Lexeme *args = evalArgs(cdr(tree), env);
	Lexeme *senv = car(closure);  //definition env
	Lexeme *params = car(cdr(cdr(closure)));
	//printLex(car(args));
	//printLex(car(params));
	Lexeme *xenv = extendEnv(senv, params, args);
	Lexeme *body = cdr(cdr(cdr(closure)));
	//printLex(body);
	//printLex(car(body));
	//printLex(car(car(body)));
	//printLex(cdr(body));
	//printLex(car(cdr(body)));
	printEnv(senv);
	printEnv(xenv);
	return eval(body, xenv);
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
	else if (tree->type == INTEGER)
		return tree;
	else if (tree->type == REAL)
		return tree;
	else if (tree->type == STRING)
		return tree;
	else if (tree->type == PROGRAM){
		eval(car(tree), env);
		eval(cdr(tree), env);
		return car(tree);
	}
	else if (tree->type == FUNCDEF)
		return evalFuncDef(tree, env);
	else if (tree->type == FUNCCALL)
		return evalFuncCall(tree, env);
	else if (tree->type == VARDEF)
		return evalVarDef(tree, env);
	else if (tree->type == RETURNSTATEMENT){
		//printLex(car(tree));
		//printLex(eval(car(tree), env));
		return eval(car(tree), env);
	}
	else if (tree->type == ARRDEF)
		return evalArrDef(tree, env);
	else if (tree->type == ARRCALL)
		return evalArrCall(tree, env);
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


