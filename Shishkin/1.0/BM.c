#include <stdio.h>
#include <string.h>

int firstread(unsigned char hay[], int lengthneedle, FILE *fin) {
	int i;
	int pos = 0;
	char symbol;
	int errornum = 0;
	for (i = 0; i < lengthneedle; i++){
		symbol = getc(fin);
		if (symbol == EOF) {
			i = lengthneedle + 1;
			errornum++;
		}
		else {
			hay[pos] = symbol;
			pos++;
		}
	}
	if (errornum > 0) return 1;
	else return 0;
}

int nextread(unsigned char hay[], int lengthneedle, int move, FILE *fin) {
	char symbol;
	int errornum = 0;
	int i;
	for (i = move; i < lengthneedle; i++) {
		hay[i - move] = hay[i];
	}
	printf("%d  ", move);
	for (i=lengthneedle-move; i < lengthneedle; i++) {
		symbol = getc(fin);
		if (symbol == EOF) {
			i = lengthneedle + 1;
			errornum++;
		}
		else {
			hay[i] = symbol;
		}
	}
	if (errornum > 0) return 1;
	else return 0;
}
void booyer (unsigned char haystack[], unsigned char needle[], FILE *fout, FILE *fin, int lengthneedle) {
	int j, l, var, ascichar1;
	int table[256];
	int i;
	int move;
	for (i=0; i<256; i++) 
		table[i] = lengthneedle;
		
	for(i=lengthneedle-2; i>=0; i--){
		ascichar1 = needle[i];
		if (table[ascichar1] == lengthneedle) table[ascichar1] = lengthneedle-1-i;
	}
	
	int result = firstread(haystack, lengthneedle, fin);
	//printf("%d", result);
	int start = 1;
	int count = 0;
	while (result != 1) {
		int i;
		for (i=lengthneedle-1; i>=0; i--) {
			fprintf(fout, "%d ", i+start);
			count++;
			if (haystack[i] != needle[i]) break;
			
		}
		//printf("%d", result);
		//finished for
		if (i<0) {
			move = lengthneedle;
			printf("f", result);
		} 
		//exited for
		if (i>=0) {
			if (count == 1) move = table[haystack[i+1]];
			if (count > 1) move = table[needle[i]];	
			printf("g  ", count);
		}
		result = nextread(haystack, lengthneedle, move, fin);
		start = start + move;
	}
	
}


int main () {
	FILE *fin, *fout;
	unsigned char haystack[16];
	unsigned char needle[16];
	char f, symbol;
	int c, d, i, lengthhay, lengthneedle;
	fout = fopen("out.txt", "w");
	fin=fopen("in.txt", "r");

	if (fin == NULL) 
		fprintf(fout, "File could not be opened.");
	else {
		
		i = 0;
		while ((symbol=getc(fin)) != '\n'){
			needle[i] = symbol;
 			i++;
		}
		lengthneedle = i;

		booyer(haystack, needle, fout, fin, lengthneedle);


		
	}
	fclose(fin);
	fclose(fout);
	return 0; 
}
