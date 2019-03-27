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
//	printf("trying to print: ");
//	printLex(args);
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
		evalPrint(eval(args, env), env);
	}
	else if (args->type == RETURNSTATEMENT){
		printf("args is return statement???\n");
		evalPrint(car(args), env);
       	}
	else if (args->type == ARRCALL)
		evalPrint(eval(args, env), env);
	//else
		//printLex(args);
	return args;
}

Lexeme *evalPlus(Lexeme *tree, Lexeme *env){
	printf("INSIDE EVALPLUS\n");
	printEnv(env);
	printf("adding eval of these 2 args...\n");
	printLex(car(cdr(tree)));
	printLex(car(cdr(cdr(tree))));
	Lexeme *walk = evalArgs(cdr(tree), env);
	if (car(walk)->type == INTEGER){
		printf("is integer\n");
		int sum = 0;
		while (walk != NULL){
			sum += car(walk)->integer;
			walk = evalArgs(cdr(walk), env);
		}
		Lexeme *l = newIntLexeme(INTEGER, sum);
		printf("evalPlus is returning: ");
		printLex(l);
		return l;
	}
	else if (car(walk)->type == REAL){
		double sum = 0.0;
		while (walk != NULL){
			sum += car(walk)->real;
			walk = evalArgs(cdr(walk), env);
		}
		return newRealLexeme(REAL, sum);
	}
	else if (car(walk)->type == FUNCCALL){
		printf("xxxxxxxxxxx\n");
		//Lexeme *newArgs = evalArgs(walk, env);
		//return evalPlus(cons(GLUE, NULL, newArgs), env);
	}
	else{
		printf("what is this lexeme? ");
		printLex(car(walk));
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
	else if (cdr(cdr(tree)) == NULL){
		printf("fatal error near line %d: too few args to assign\n", car(tree)->line);
		exit(-1);
	}
	else if (cdr(cdr(cdr(tree))) != NULL){
		// assign 3rd arg to 2nd arg index
		Lexeme *arrdef = getVar(env, car(cdr(tree)));
		Lexeme *var = cdr(arrdef);
		int index = car(cdr(cdr(tree)))->integer;
		int i = 1;
		while (i < index && var != NULL){
			var = cdr(var);
			i++;
		}
		var->left = car(cdr(cdr(cdr(tree))));  //doesn't like when I use car on the left of the assign  //doesn't like when I use car on the left of the assign
	}
	else{
		updateVar(env, car(cdr(tree)), car(evalArgs(cdr(cdr(tree)), env)));
	}
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
	int i = 1;
	while (i < index && val != NULL){
		val = cdr(val);
		i++;
	}
	return eval(car(val), env);
}

Lexeme *evalArgs(Lexeme *tree, Lexeme *env){ 
	if (tree == NULL || tree == NULL)
		return NULL;
//	printf("trying to eval args: ");
//	printLex(tree);
//	printLex(car(tree));
	Lexeme *l = eval(car(tree), env);
//	printLex(l);

	return cons(ARGS, l, evalArgs(cdr(tree), env));
}

Lexeme *evalID(Lexeme *tree, Lexeme *env){
	//printf("INSIDE EVALID\n");
	//printf("looking for: ");
	//printLex(tree);
	//printEnv(env);
	Lexeme *l = getVar(env, tree);
	//printLex(l);
	return l;
}

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env){
//	printf("INSIDE EVALFUNCCALL\n");
//	printf("looking for: ");
//	printLex(car(tree));
	if (isBuiltIn(tree))
		return evalBuiltIn(tree, env);
	Lexeme *closure = getVar(env, car(tree));
	Lexeme *args = evalArgs(cdr(tree), env);
	Lexeme *senv = car(closure);  //definition env
//	printLex(senv);
	Lexeme *params = car(cdr(cdr(closure)));
	Lexeme *xenv = extendEnv(senv, params, args);
//	printEnv(senv);
//	printf("xenv...\n");
//	printEnv(xenv);
	Lexeme *body = cdr(cdr(cdr(closure)));
//	printf("calling eval(body, senv), body=");
//	printLex(body);
//	printf("car(body) is ");
//	printLex(car(body));
//	printf("cdr(body) is ");
//	printLex(cdr(body));
	printf("eval(body, xenv)...\n");
	Lexeme *l = eval(body, xenv);
	printf("result of evaling body is: ");
	printLex(l);
	return l;
}

Lexeme *eval(Lexeme *tree, Lexeme *env){
	if (tree == NULL)
		return NULL;
	else if (tree->type == INTEGER)
		return tree;
	else if (tree->type == REAL)
		return tree;
	else if (tree->type == STRING)
		return tree;
	else if (tree->type == PROGRAM){
		Lexeme *lcar = eval(car(tree), env);
		Lexeme *lcdr = eval(cdr(tree), env);
		if (lcdr == NULL)
			return lcar;
		else	
			return lcdr;
		//return NULL;// car(tree);
	}
	else if (tree->type == FUNCDEF)
		return evalFuncDef(tree, env);
	else if (tree->type == FUNCCALL)
		return evalFuncCall(tree, env);
	else if (tree->type == VARDEF)
		return evalVarDef(tree, env);
	else if (tree->type == RETURNSTATEMENT){
		printf("evaluating RETURNSTATEMENT: ");
		printLex(tree);
		printf("car(returnstatement) is: ");
		printLex(car(tree));
		Lexeme *l = eval(car(tree), env);
		printf("evaluation of RETURNSTATEMENT is: ");
		printLex(l);
		//printLex(car(car(tree)));
		return l;
	}
	else if (tree->type == ARRDEF)
		return evalArrDef(tree, env);
	else if (tree->type == ARRCALL)
		return evalArrCall(tree, env);
	else if (tree->type == ID)
		return evalID(tree, env);
	//...
	else{
		fprintf(stderr, "internal eval error\n");
		printLex(tree);
		exit(-1);
	}
}


