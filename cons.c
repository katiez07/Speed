// cons functions
// @author Katie Zucker

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"

Lexeme *cons(Lexeme *tree, Lexeme *left, Lexeme *right){
	tree->left = left;
	tree->right = right;
	return tree;
}

Lexeme *car(Lexeme *tree){
	return tree->left;
}

Lexeme *cdr(Lexeme *tree){
	return tree->right;
}

Lexeme *cadr(Lexeme *tree){
	return tree->right->left;
}

Lexeme *cddr(Lexeme *tree){
	return tree->right->right;
}


