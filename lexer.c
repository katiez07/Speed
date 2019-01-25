// @author Katie Zucker

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


char pushChar;
int charPushed = 0;
FILE *input;

char read(){
	return fgetc(input);
}

char pushback(ch){
	if (charPushed){
		charPushed = 0;
		return pushChar;
	}
	return read();
}

void skipWhiteSpace(){
	char ch;
	while (isspace(ch))
		ch = read();

	pushback(ch);
}


int main (int argc, char **argv){
	input = fopen(argv[1], "r");
	
	while (!feof(input)){
		printf("%c", read());
	}
	
	fclose(input);
	
	return 0;
}
