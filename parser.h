// @author Katie Zucker
// parser header

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern Lexeme *program();
extern Lexeme *expr();
extern Lexeme *args();
extern Lexeme *optArgs();
extern Lexeme *ifStatement();
extern Lexeme *elseStatement();
extern Lexeme *optElseStatement();
extern Lexeme *def();
extern Lexeme *structDef();
extern Lexeme *varDef();
extern Lexeme *funcDef();
extern Lexeme *returnStatement();
extern Lexeme *unary();
extern Lexeme *IDexpr();
