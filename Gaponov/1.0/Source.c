#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STR_LEN 256
#define MAX_PAT_LEN 17

void BMSearch(unsigned char*);
void GetString(unsigned char*);
void BMSearch(unsigned char *);
void Search(unsigned char *, unsigned char *, int, int[]);
int GetTextToString(unsigned char *, int, int);
int Shift(unsigned char *, int, int);

int main() {
	unsigned char pat[MAX_PAT_LEN];
	GetString(pat);
	BMSearch(pat);
	return 0;
}

void GetString(unsigned char *str) {
	int c;
	int i = 0;
	for (; (c = getc(stdin)) != '\n'; ++i)
		str[i] = c;
	str[i] = '\0';
}

int Shift(unsigned char *str, int strLength, int shiftSize) {
	if (strLength - 1 - shiftSize >= 0)
		for (int i = 1; i < strLength; ++i)
			if ((i - shiftSize) >= 0)
				str[i - shiftSize] = str[i];

	if (GetTextToString(str, strLength, strLength - shiftSize) == EOF) return EOF;
	return shiftSize;
}

void Search(unsigned char *pat, unsigned char *stringToCompare, int patLen, int shiftTable[]) {
	int c = 1;
	int shift,i;
	while (1) {  
		for (i = patLen - 1; i >= 0; --i) {
			fprintf(stdout, "%d ", c + i); 
			if (stringToCompare[i] != pat[i]) {
				if (i == patLen - 1)
					shift = shiftTable[stringToCompare[i]];
				else
					shift = shiftTable[pat[patLen - 1]];
				if (Shift(stringToCompare, patLen, shift) != EOF) {
					c += shift;
					break;
				}
				else return;
			}
		}
		if (i < 0) {
			if (Shift(stringToCompare, patLen, patLen) == EOF) return;
			else c += patLen;
		}
	}
}

void BMSearch(unsigned char *pat) {
	int patLen = strlen(pat);
	int shiftTable[STR_LEN];

	int i = 0;
	for (i = 0; i < STR_LEN; ++i)
		shiftTable[i] = patLen;
	for (i = 0; i < patLen - 1; ++i)
		shiftTable[pat[i]] = patLen - i - 1;

	unsigned char *stringToCompare = (unsigned char*)malloc(sizeof(unsigned char) * patLen);

	if (GetTextToString(stringToCompare, patLen, 0) == EOF) {
		free(stringToCompare);
		return;
	}

	Search(pat, stringToCompare, patLen, shiftTable);

	free(stringToCompare);
}

int GetTextToString(unsigned char *str, int strLen, int stPos) {
	int c;
	for (int i = stPos; i < strLen; ++i) {
		if ((c = getc(stdin)) != EOF)
			str[i] = c;
		else
			return EOF;
	}
	return strLen - stPos;
}