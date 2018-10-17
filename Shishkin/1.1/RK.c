#include <stdio.h>
#include <string.h>
void fprintarr (int start, int length, FILE *fout) {
	int i;
	for (i=start; i<start+length; i++) fprintf(fout, "%d ", i+1);
}

int read (char hay[], int start, int finish, FILE *fin) {
	int i;
	int pos = 0;
	char symbol;
	for (i=start; i<= finish; i++){
		symbol = getc(fin);
		if(symbol == EOF) {
			i = finish+1;
			return 1;
		}
		else {
			hay[pos] = symbol;
			pos++;
			return 0;
		}
	}
}

int hash (char array[], int fp, int lp) {
	int result = 0;
	int i;
	for (i=fp; i<=lp; i++) {
		result = result + (((array[i]%3)*(3^i)));
	}
	return result;
}

int compare (char needle[], char hay[], int lengthneedle, int start) {
	int sim = 0;
	int i, hsned, hshay;
	hsned = hash(needle, 0, lengthneedle-1);
	hshay = hash(hay, start, lengthneedle-1+start);
	
	if (hsned == hshay) {
		for (i=0; i<lengthneedle; i++) {
			if (needle[i] == hay[i+start]) {
			sim++;
			}
		}
	if (sim == lengthneedle) return 0;
	else return 1; 
	}
}
void rabinkarp (char needle[], char hay[], int lengthneedle, FILE *fin, FILE *fout) {
	int readres, compareres;
	int finish = lengthneedle - 1;
	int start = 0;
	readres = read (hay, start, finish, fin);
	while (readres != 1) {
		compareres = compare(needle, hay, lengthneedle, start);
		if (compareres == 0) {
			fprintarr(start, lengthneedle, fout);
			fprintf(fout, "%d ", hash(needle, 0, lengthneedle-1));
		}
		start = start + lengthneedle;
		finish = finish + lengthneedle;
		readres = read (hay, start, finish, fin);
	}
}

int main() {
	FILE *fin, *fout;
	
	char needle[16];
	char haystack[16];
	char f, symbol;
	int c, d, i, lengthneedle;
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
		rabinkarp (needle, haystack, lengthneedle, fin, fout);	
		
			
	}
	fclose(fin);
	fclose(fout);
	return 0; 

}
