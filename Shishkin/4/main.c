#include "calc.h"
#include <stdio.h> 

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
		int error = 0;
		int result = calc(symbols, amount, &error);
		if (error == 1) fprintf(fout, "syntax error");
		if (error == 2) fprintf(fout, "division by zero");
		if (error == 0) fprintf(fout, "%d", result);
						
	}
	return 0;
}
