// eval header
// @author Katie zucker

#include <stdio.h>
#include <ctype.h>

int clCount;
char **clArgs;

Lexeme *eval(Lexeme *, Lexeme *);
Lexeme *evalArgs(Lexeme *, Lexeme *);
Lexeme *evalID(Lexeme *, Lexeme *);
