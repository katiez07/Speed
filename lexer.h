// 2author Katie Zucker

#include <ctype.h>
#include <string.h>

typedef enum{
	ID,
	INTEGER,
	REAL,
	STRING,
	SEMI,
	OPAREN,
	CPAREN,
	OBRACE,
	CBRACE,
	OBRACKET,
	CBRACKET,
	STRUCTURE,
	FUNCTION,
	VARIABLE,
	IF,
	ELSE,
	RETURN,
	QUOTE
} types;

typedef struct{
	char *type;
	char *string;
	int integer;
	double real;
	int line;
} Lexeme;


