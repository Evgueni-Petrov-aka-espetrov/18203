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
			//printf(" %d --- %c ", i, symbol);
			pos++;
		}
	}
	//printf("\n");
	if (errornum > 0) return 1;
	else return 0;
}
int nextread(unsigned char hay[], int lengthneedle, FILE *fin) {
	int i;
	char symbol;
	int errornum = 0;
	for (i = 1; i < lengthneedle; i++) {
		hay[i - 1] = hay[i];
	}

	symbol = getc(fin);
	if (symbol == EOF) {
		i = lengthneedle + 1;
		errornum++;
	}
	else {
		hay[lengthneedle - 1] = symbol;
		//printf(" %d --- %c ", i, symbol);
	}
	if (errornum > 0) return 1;
	else return 0;
}
int power(int a, int n) {
	int i;
	int t;
	t = 1;
	for (i = 1; i <= n; i++) {
		t = t*a;
	}
	return t;
}
int hash(unsigned char array[], int lengthneedle) {
	int result = 0;
	int i;
	for (i = 0; i <= lengthneedle - 1; i++) {
		//printf("  %d  ", result);
		result = result + ((array[i] % 3)*power(3, i));
		//printf("  %d  ", result);
	}
	//printf("\n");
	return result;
}

void compare(unsigned char needle[], unsigned char hay[], int start, int lengthneedle, FILE *fout) {
	int sim = 0;
	int i, hsned, hshay;
	//printf("start  ");
	hsned = hash(needle, lengthneedle);
	//printf("  stop  ");
	hshay = hash(hay, lengthneedle);

	if (hsned == hshay) {
		//for (i = 0; i<=lengthneedle-1; i++) {
		//	if (needle[i] == hay[i]) {
		//		fprintf(fout, "%d ", i + 1 + start);
		//	}
		i = 0;
		do {
			fprintf(fout, "%d ", i + 1 + start);
			i++;
		} while ((i <= lengthneedle - 1) && (needle[i-1] == hay[i-1]));
		}	

	}

void rabinkarp(unsigned char needle[], unsigned char hay[], int lengthneedle, FILE *fin, FILE *fout) {
	int readres, compareres;
	//int finish = lengthneedle - 1;
	//printf("%d", finish);
	int start = 0;
	fprintf(fout, "%d ", hash(needle, lengthneedle));
	readres = firstread(hay, lengthneedle, fin);
	while (readres != 1) {
		compare(needle, hay, start, lengthneedle, fout);
		//printf("hfgjgkj");
		//if (compareres == 0) {


		//fprintarr(start, lengthneedle, fout);
		//}
		start = start + 1;
		//finish = finish + 1;
		readres = nextread(hay, lengthneedle, fin);
	}
}

int main() {
	FILE *fin, *fout;

	unsigned char needle[16];
	unsigned char haystack[16];
	unsigned char f, symbol;
	int c, d, i, lengthneedle;
	fout = fopen("out.txt", "w");
	fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		i = 0;
		while ((symbol = getc(fin)) != '\n'){
			needle[i] = symbol;

			i++;
		}
		lengthneedle = i;
		rabinkarp(needle, haystack, lengthneedle, fin, fout);


	}
	fclose(fin);
	fclose(fout);
	return 0;

}
