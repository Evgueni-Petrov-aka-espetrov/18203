#include <stdio.h> 
#include <string.h> 
#include <malloc.h>
#include <assert.h>
#include "calc.h"

int readfile(FILE *fin, char symbols[]) {
	int i = 0;
	char symbol;
	while ((symbol = getc(fin)) != EOF){
		symbols[i] = symbol;
		i++;
	}
	return i-1;
}

int main() {
	FILE *fout = fopen("out.txt", "w");
	FILE *fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		char symbols[MAX_LEN];
		int amount = readfile(fin, symbols);
		calc(symbols, amount, fout);
		
			
		
	}
	return 0;
}
