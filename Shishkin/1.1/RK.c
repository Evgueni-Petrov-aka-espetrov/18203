#include <stdio.h>
#include <string.h>
void fprintarr(int start, int length, FILE *fout) {
	int i;
	for (i = start; i<start + length; i++) fprintf(fout, "%d ", i + 1);
}

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

int nextread(unsigned char hay[], int lengthneedle, FILE *fin) {
	char symbol;
	int errornum = 0;
	int i;
	for (i = 1; i < lengthneedle; i++) {
		hay[i - 1] = hay[i];
	}
	symbol = getc(fin);
	if (symbol == EOF) {
		errornum++;
	}
	else {
		hay[lengthneedle - 1] = symbol;
	}
	if (errornum > 0) return 1;
	else return 0;
}

int power(int n) {
	int powersof3[16] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907};
	return powersof3[n];
}

int hash(unsigned char array[], int lengthneedle) {
	int result = 0;
	int i;
	for (i = 0; i <= lengthneedle - 1; i++) {
		result = result + ((array[i] % 3)*power(i));
	}
	return result;
}

void compare(unsigned char needle[], unsigned char hay[], int start, int lengthneedle, int defhash, int hayhash, FILE *fout) {
	if (defhash == hayhash) {
		int i = 0;
		do {
			fprintf(fout, "%d ", i + 1 + start);
			i++;
		} while ((i <= lengthneedle - 1) && (needle[i-1] == hay[i-1]));
	}	
}

void rabinkarp(unsigned char needle[], unsigned char hay[], int lengthneedle, FILE *fin, FILE *fout) {
	int start = 0;
	int defhash = hash(needle, lengthneedle);
	float parthash;
	fprintf(fout, "%d ", defhash);
	int readres = firstread(hay, lengthneedle, fin);
	int hayhash = hash(hay, lengthneedle);
	while (readres != 1) {		
		compare(needle, hay, start, lengthneedle, defhash, hayhash, fout);
		start++;
		parthash = (hayhash - hay[0]%3)/3;
		readres = nextread(hay, lengthneedle, fin);
		hayhash = parthash + (hay[lengthneedle-1]%3)*power(lengthneedle-1);
	}
}

int main() {
	FILE* fout = fopen("out.txt", "w");
	FILE* fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		unsigned char needle[16];
		unsigned char haystack[16];
		unsigned char symbol;
		int i = 0;
		while ((symbol = getc(fin)) != '\n'){
			needle[i] = symbol;
			i++;
		}
		int lengthneedle = i;
		rabinkarp(needle, haystack, lengthneedle, fin, fout);
	}
	fclose(fin);
	fclose(fout);
	return 0;

}
