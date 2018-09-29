#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define S_TAB_LEN 256
#define TRUE 1

int GetTextToString(unsigned char *str, int strLength, int startPos) {
	for (register int i = startPos; i < strLength; ++i) {
		register int c;
		if ((c = getc(stdin)) != EOF) {
			str[i] = c;
		}
		else {
			return EOF;
		}
	}
	return strLength - startPos; // return number of filled characters
}

void BuildShiftTable(unsigned char *pattern, int patLength, int shiftTable[]) {
	for (register int i = 0; i < S_TAB_LEN; ++i) {
		shiftTable[i] = patLength;
	}
	for (register int i = 0; i < patLength - 1; ++i) {
		shiftTable[pattern[i]] = patLength - i - 1;
	}
}

int Shift(unsigned char *str, int strLength, int shiftSize) {
	// if shift isn't necessary just fill the string
	if (strLength - 1 - shiftSize >= 0) {
		for (register int i = 1; i < strLength; ++i) {
			if ((i - shiftSize) >= 0) {
				str[i - shiftSize] = str[i];
			}
		}
	}
	if (GetTextToString(str, strLength, strLength - shiftSize) == EOF) return EOF;
	return shiftSize;
}

void BMSearchMainPart(unsigned char *pattern, unsigned char *stringToCompare, int patLength, int shiftTable[]) {
	register int count = 1; // This var needs for logging
	while (TRUE) {  // exit from this loop when the text ends;
		register int i;
		for (i = patLength - 1; i >= 0; --i) {
			fprintf(stdout, "%d ", count + i); // logging
			if (stringToCompare[i] != pattern[i]) {
				register int shift = (i == patLength - 1) ? shiftTable[stringToCompare[i]] : shiftTable[pattern[patLength - 1]];
				if (Shift(stringToCompare, patLength, shift) != EOF) {
					count += shift;
					break;
				}
				else return;
			}
		}
		// if pattern is found shift it on it's length
		if (i < 0) {
			if (Shift(stringToCompare, patLength, patLength) == EOF) return;
			else count += patLength;
		}
	}
}

void BMSearch(unsigned char *pattern) {
	int patLength = strlen(pattern);
	// 1st step - build a shift table
	int shiftTable[S_TAB_LEN];
	BuildShiftTable(pattern, patLength, shiftTable);
	// The pattern is comparing with the text in this string
	unsigned char *stringToCompare = (unsigned char*)malloc(sizeof(unsigned char) * patLength);
	// Fill stringToCompare from the text
	if (GetTextToString(stringToCompare, patLength, 0) == EOF) {
		free(stringToCompare);
		return;
	}
	// 2nd step - main algorithm
	BMSearchMainPart(pattern, stringToCompare, patLength, shiftTable);

	free(stringToCompare);
}
