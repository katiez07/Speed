// @author Katie Zucker

#include <stdio.h>
#include <string.h>
#include "lexer.h"

char pushChar;
int charPushed = 0;

char pushback(ch){
	if (charPushed){
		charPushed = 0;
		return pushChar;
	}
	else
		return Input.read();
}

void skipWhiteSpace(){
	var ch1;
	while (isWhiteSpace(ch))
		ch = Input.read();

	Input.pushback(ch);
}

int main (int argc, char **argv){
	FILE *fp;
	fp = fopen(r, argv[1]);
	fopen(fp);
	
	
	
	fclose(fp);
	
	return 0;
}
