#include <stdio.h>
#include <string.h>

int read(FILE *fin, unsigned char hay[], int move, int lengthneedle){
	int errornum = 0;
	int i;
	for (i = lengthneedle - move; i < lengthneedle; i++) {
		char symbol;
		symbol = getc(fin);
		if (symbol == EOF) {
			i = lengthneedle + 1;
			errornum++;
		}
		else {
			hay[i] = symbol;
		}
	}
	return errornum;
}
int firstread(unsigned char hay[], int lengthneedle, FILE *fin) {
	//int i;
	int errornum = 0;
	//for (i = 0; i < lengthneedle; i++){
	//	char symbol;
	//	symbol = getc(fin);
	//	if (symbol == EOF) {
	//		i = lengthneedle + 1;
	//		errornum++;
	//	}
	//	else {
	//		hay[i] = symbol;
	//	}
	//}
	errornum = read(fin, hay, lengthneedle, lengthneedle);
	if (errornum > 0) return 1;
	else return 0;
}

int nextread(unsigned char hay[], int lengthneedle, int move, FILE *fin) {
	int errornum = 0;
	int i;
	for (i = move; i < lengthneedle; i++) {
		hay[i - move] = hay[i];
	}
	//for (i = lengthneedle - move; i < lengthneedle; i++) {
	//	char symbol;
	//	symbol = getc(fin);
	//	if (symbol == EOF) {
	//		i = lengthneedle + 1;
	//		errornum++;
	//	}
	//	else {
	//		hay[i] = symbol;
	//	}
	//}
	errornum = read(fin, hay, move, lengthneedle);
	if (errornum > 0) return 1;
	else return 0;
}
void booyer(unsigned char haystack[], unsigned char needle[], FILE *fout, FILE *fin, int lengthneedle) {
	int table[256];
	int i;
	for (i = 0; i<256; i++)
		table[i] = lengthneedle;
	for (i = lengthneedle - 2; i >= 0; i--){
		int ascichar1;
		ascichar1 = needle[i];
		if (table[ascichar1] == lengthneedle) table[ascichar1] = lengthneedle - 1 - i;
	}
	int result = firstread(haystack, lengthneedle, fin);
	int start = 1;
	while (result != 1) {
		int i;
		int count = 0;
		for (i = lengthneedle - 1; i >= 0; i--) {
			fprintf(fout, "%d ", i + start);
			count++;
			if (haystack[i] != needle[i]) break;
		}
		int move;
		//finished for
		if (i<0) {
			move = lengthneedle;
		}
		//exited for
		if (i >= 0) {
			if (count == 1) move = table[haystack[i]];
			if (count > 1) move = table[needle[lengthneedle-1]];
		}
		result = nextread(haystack, lengthneedle, move, fin);
		start = start + move;
	}
}

int main() {
	FILE *fout = fopen("out.txt", "w");
	FILE *fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		unsigned char haystack[16];
		unsigned char needle[16];
		char symbol;
		int lengthneedle;
		int i = 0;
		while ((symbol = getc(fin)) != '\n'){
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