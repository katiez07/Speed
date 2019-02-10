// @author Katie Zucker
// test for envivronment module

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "environment.h"

int main(){
	
	cons(TAB, NULL, NULL);
	Lexeme *x = cons(TAB, NULL, NULL);
	Lexeme *y = cons(V, x, x);
	setCar(y, x);

	Lexeme *env = newEnv();
	setCar(env, NULL);
//	setCar(env, x);

	/*
	Lexeme *env = newEnv();
	env->left = newLexeme(OPAREN);
	*/

	/*
	Lexeme *lvar0 = newLexeme(ID);
	Lexeme *lval0 = newIntLexeme(INTEGER, 4);
	insertEnv(env, lvar0, lval0);
	*/

	return 0;	
}
