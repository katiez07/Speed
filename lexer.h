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
	TRUE,
	FALSE,
	ELSE,
	RETURN,
	ID,
	INTEGER,
	REAL,
	STRING,
	STRUCT,
	FUNCTION,
	VARIABLE,
	BAD_NUMBER,
	BAD_LEXEME
} types;

typedef struct{
	types type;
	char *string;
	int integer;
	double real;
	int line;
} Lexeme;


FILE *input;

extern Lexeme *lex();

