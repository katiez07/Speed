// @author Katie Zucker

#include <ctype.h>

typedef enum{
	OBRACKET,
	CBRACKET,
	SEMI,
	OPAREN,
	CPAREN,
	OBRACE,
	CBRACE,
	IF,
	ELSE,
	RETURN,
	ID,
	INTEGER,
	REAL,
	STRING,
	STRUCT,
	FUNCTION,
	VARIABLE,
	ENDOFFILE,
	BAD_NUMBER,
	BAD_LEXEME,
	PROGRAM,
	EXPR,
	OPTEXPR,
	ARGS,
	OPTARGS,
	PARAMS,
	OPTPARAMS,
	IFELSETOP,
	IFSTATEMENT,
	ELSESTATEMENT,
	OPTELSESTATEMENT,
	DEF,
	STRUCTDEF,
	VARDEF,
	FUNCDEF,
	RETURNSTATEMENT,
	UNARY,
	IDEXPR,
	FUNCCALL,
	GLUE,
	ENV,
	TAB,
	I,
	V, 
	CLOSURE,
	NULLVALUE,
	ARRAY,
	ARRDEF,
	ARRCALL
} types;

typedef struct Lexeme {
	types type;
	char *id;
	char *string;
	int integer;
	double real;
	int line;
	struct Lexeme *left;
	struct Lexeme *right;
} Lexeme;


FILE *input;
int linenum;
Lexeme *curlex;

extern Lexeme *lex();
extern void skipWhiteSpace();

extern Lexeme *newLexeme(types);
extern Lexeme *newStringLexeme(types, char *);
extern Lexeme *newIntLexeme(types, int);
extern Lexeme *newRealLexeme(types, double);
extern Lexeme *newIDLexeme(types, char *);

extern Lexeme *cons(types, Lexeme *, Lexeme *);
extern Lexeme *car(Lexeme *);
extern Lexeme *cdr(Lexeme *);
extern Lexeme *setCar(Lexeme *, Lexeme *);
extern Lexeme *setCdr(Lexeme *, Lexeme *);
extern Lexeme *cadr(Lexeme *);
extern Lexeme *cddr(Lexeme *);
