// @author Katie Zucker
// environment header

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern Lexeme *newEnv();
extern Lexeme *insertEnv(Lexeme *, Lexeme *, Lexeme *);
extern Lexeme *getVar(Lexeme *, Lexeme *);
extern Lexeme *updateVar(Lexeme *, Lexeme *, Lexeme *);
extern void printLocalEnv(Lexeme *);
extern void printEnv(Lexeme *);


