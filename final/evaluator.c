// eval
// @author Katie Zucker

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "environment.h"
#include "evaluator.h"

int clCount;
char **clArgs;

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
		|| strcmp(id, "=") == 0
		|| strcmp(id, "==") == 0
		|| strcmp(id, "ofiler") == 0
		|| strcmp(id, "readint") == 0
		|| strcmp(id, "eof") == 0
		|| strcmp(id, "cfile") == 0
		|| strcmp(id, "getCLArgCount") == 0
		|| strcmp(id, "getCLArg") == 0)
		return 1;
	return 0;
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
	else if (args->type == LAMBDA)
		printf("lambda");
	else if (args->type == CLOSURE){
		printLex(car(cdr(args)));
		if (car(cdr(args))->type == LAMBDA){
			printf("CLOSURE: ");
			if (car(cdr(args)) == NULL){
				printf("lambda (");
			}
			else{
				printf("%s (", car(cdr(args))->id);
			}
			Lexeme *ar = car(cdr(cdr(args)));
			while (ar != NULL){
				printf("%s ", ar->id);
				ar = cdr(ar);
			}
			printf(") ");
			printf("./ ");
			evalPrint(car(cdr(cdr(cdr(args)))), env);
			printf("... /.");
		}
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
			walk = evalArgs(cdr(walk), env);
		}
		Lexeme *l = newIntLexeme(INTEGER, sum);
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

Lexeme *evalEq(Lexeme *tree, Lexeme *env){
	Lexeme *match = eval(car(cdr(tree)), env); //1st param to match with
	Lexeme *args = cdr(tree);
	if (match->type == INTEGER){
		while (args != NULL){
			if (eval(car(args), env)->integer != match->integer)
				return newIntLexeme(INTEGER, 0);
			args = cdr(args);
		}
		return newIntLexeme(INTEGER, 1);
	}
	else if (match->type == REAL){
		while (args != NULL){
			if (eval(car(args), env)->real != match->real)
				return newIntLexeme(INTEGER, 0);
			args = cdr(args);
		}
		return newIntLexeme(INTEGER, 1);
	}
	else if (match->type == STRING){
		while (args != NULL){
			if (strcmp(eval(car(args), env)->string, match->string) != 0)
				return newIntLexeme(INTEGER, 0);
			args = cdr(args);
		}
		return newIntLexeme(INTEGER, 1);
	}
	else{
		return NULL;
	}
}

Lexeme *evalOpenFileRead(Lexeme *f, Lexeme *env){
	// I couldn't figure out how the notes do this
	// so I figured out my own strategy
	// my strategy is to put the file the env like a var; I call it hacker style
	Lexeme *lfileid = newIDLexeme(ID, f->string);
	Lexeme *lfp = newFileLexeme(FILE_POINTER, fopen(f->string, "r"));
	insertEnv(env, lfileid, lfp);
	
	return lfp;
}

int freadint(FILE *fp){
	//char ch = fgetc(fp);
	char ch;
	char *s = malloc(sizeof(char *));
	int i = 0;
	ch = fgetc(fp);
	
	if (ch <= 47 || ch >= 58){
		while ((ch <= 47 || ch >= 58) && !feof(fp)){
			ch = fgetc(fp); 
		}
	}

	while ((ch > 47 && ch < 58) && !feof(fp)){
		s[i] = ch;
		i++;
		ch = fgetc(fp);
	}
	return atoi(s);
}

// doing files hacker style :)
// if you're confused about what this is doing, uncomment stuff here
Lexeme *evalReadInt(Lexeme *l, Lexeme *env){
	int x;
	//printEnv(env);
	//printLex(l);
	//printf("looking for: ");
	//printLex(car(cdr(l)));
	Lexeme *evaledArgs = eval(car(cdr(l)), env);
	Lexeme *fid2 = newIDLexeme(ID, evaledArgs->string);
	//printf("converting to: ");
	//printLex(fid2);
	Lexeme *lfile = getVar(env, fid2);
	x = freadint(lfile->file);
	//printf("%d\n", x);
	return newIntLexeme(INTEGER, x);
}

// also hacker style for eof
Lexeme *evalEof(Lexeme *l, Lexeme *env){
	Lexeme *evaledArgs = eval(l, env);
	Lexeme *fid2 = newIDLexeme(ID, evaledArgs->string);
	Lexeme *lfile = getVar(env, fid2);
	if (feof(lfile->file))
		return newIntLexeme(INTEGER, 1);
	else
		return newIntLexeme(INTEGER, 0);
}

// and hacker style for fclose
Lexeme *evalCloseFile(Lexeme *l, Lexeme *env){
	Lexeme *evaledArgs = eval(l, env);
	Lexeme *fid2 = newIDLexeme(ID, evaledArgs->string);
	Lexeme *lfile = getVar(env, fid2);
	fclose(lfile->file);
	return newIntLexeme(INTEGER, 1);
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
	else if (strcmp(id, "==") == 0)
		return evalEq(tree, env);
	else if (strcmp(id, "ofiler") == 0){
		Lexeme *l = eval(car(cdr(tree)), env);
		return evalOpenFileRead(l, env);
	}
	else if (strcmp(id, "readint") == 0){
		return evalReadInt(tree, env);
	}
	else if (strcmp(id, "eof") == 0)
		return evalEof(car(cdr(tree)), env);
	else if (strcmp(id, "cfile") == 0)
		return evalCloseFile(car(cdr(tree)), env);
	else if (strcmp(id, "getCLArgCount") == 0)
		return newIntLexeme(INTEGER, clCount-1);
	else if (strcmp(id, "getCLArg") == 0){	
		Lexeme *lindex = eval(car(cdr(tree)), env);
		int index = lindex->integer;
		char *s = clArgs[index+1];
		return newStringLexeme(STRING, s);
	}
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
	Lexeme *l = eval(car(tree), env);

	return cons(ARGS, l, evalArgs(cdr(tree), env));
}

Lexeme *evalID(Lexeme *tree, Lexeme *env){
	Lexeme *l = getVar(env, tree);
	return l;
}

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env){
	if (isBuiltIn(tree))
		return evalBuiltIn(tree, env);
	Lexeme *closure = getVar(env, car(tree));
	Lexeme *args = evalArgs(cdr(tree), env);
	Lexeme *senv = car(closure);  //definition env
	Lexeme *params = car(cdr(cdr(closure)));
	Lexeme *xenv = extendEnv(senv, params, args);
	Lexeme *body = cdr(cdr(cdr(closure)));
	Lexeme *l = eval(body, xenv);
	return l;
}

Lexeme *evalIfElse(Lexeme *tree, Lexeme *env){
	Lexeme *cond1, *body1, *ie2;
	cond1 = car(car(tree));
	body1 = cdr(car(tree));
	ie2 = car(cdr(tree));
	if (eval(cond1, env)->integer == 1)
		return eval(body1, env);
	else if (ie2->type == ELSESTATEMENT){
		Lexeme *body2 = cdr(car(cdr(tree)));
		return eval(body2, env);
	}
	else //(ie2 == IFSTATEMENT)
		return evalIfElse(cdr(tree), env);
}

Lexeme *evalLambda(Lexeme *tree, Lexeme *env){
	return cons(CLOSURE, env, tree);
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
	}
	else if (tree->type == FUNCDEF)
		return evalFuncDef(tree, env);
	else if (tree->type == FUNCCALL)
		return evalFuncCall(tree, env);
	else if (tree->type == VARDEF)
		return evalVarDef(tree, env);
	else if (tree->type == RETURNSTATEMENT){
		Lexeme *l = eval(car(tree), env);
		return l;
	}
	else if (tree->type == ARRDEF)
		return evalArrDef(tree, env);
	else if (tree->type == ARRCALL)
		return evalArrCall(tree, env);
	else if (tree->type == ID)
		return evalID(tree, env);
	else if (tree->type == IFELSETOP)
		return evalIfElse(tree, env);
	else if (tree->type == LAMBDA){
		return evalLambda(tree, env);
	}
	//...
	else{
		fprintf(stderr, "internal eval error\n");
		printLex(tree);
		exit(-1);
	}
}


