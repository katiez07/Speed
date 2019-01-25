// 2author Katie Zucker

#include <cstring.h>

typedef struct{
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
	String type;
	String string;
	int integer;
	double real;
	int line;
} Lexeme;


