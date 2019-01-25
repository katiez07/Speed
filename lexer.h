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
	BAD_STRING
} types;

typedef struct{
	char *type;
	char *string;
	int integer;
	double real;
	int line;
} Lexeme;
