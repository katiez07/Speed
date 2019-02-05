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
	BAD_LEXEME
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

