#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "boyermoore.h"
#define S_TAB_LEN 256
enum bool{ true = 1, false = 0 };
typedef unsigned char T;
typedef struct {
	T buffer[MAX_PAT_LEN];
	int position;
	int size;
} CBuffer;

int GetTextToString(unsigned char *str, int strLength) {
	int i;
	for (i = 0; i < strLength; ++i) {
		int c;
		if ((c = getc(stdin)) != EOF) {
			str[i] = c;
		}
		else {
			return EOF;
		}
	}
	return i;
}

void BuildShiftTable(const unsigned char *pattern, int patLength, int shiftTable[]) {
	for (int i = 0; i < S_TAB_LEN; ++i) {
		shiftTable[i] = patLength;
	}
	for (int i = 0; i < patLength - 1; ++i) {
		shiftTable[pattern[i]] = patLength - i - 1;
	}
}

int Add(CBuffer *buf, int shift) {
	int c;
	for (int i = 0; i < shift; ++i) {
		if ((c = getc(stdin)) != EOF) {
			if (buf->position < buf->size) {
				(buf->buffer)[buf->position] = c;
				++(buf->position);
			}
			else {
				(buf->buffer)[0] = c;
				buf->position = 1;
			}
		}
		else {
			return EOF;
		}
	}
	return shift;
}

int CompareAndLog(const CBuffer *string, const int shiftTable[], const unsigned char *pattern, int posCount) {
	int lastPosition = string->position - 1;
	for (int i = string->size - 1; i >= 0; --i, --lastPosition) {
		fprintf(stdout, "%d ", posCount + 1 + i); // logging
		if (lastPosition < 0) {
			lastPosition = string->size - 1;
		}
		if ((string->buffer)[lastPosition] != pattern[i]) {
			return 
				(i == string->size - 1) ? 
				shiftTable[(string->buffer)[lastPosition]] : 
				shiftTable[pattern[string->size - 1]];
		}
	}
	return 0;
}

void Search(const unsigned char *pattern, CBuffer *stringToCompare, int shiftTable[]) {
	int posCount = 0; // position in the text of first character of stringToCompare 
	while (true) {  // exit from this loop when the text ends;
		int shift = CompareAndLog(stringToCompare, shiftTable, pattern, posCount);
		if (shift != 0) {
			if (Add(stringToCompare, shift) == EOF) {
				break;
			}
			else {
				posCount += shift;
			}
		}
		// if pattern is found shift it on it's length
		else {
			if (Add(stringToCompare, stringToCompare->size) == EOF) {
				break;
			}
			else {
				posCount += stringToCompare->size;
			}
		}
	}
	return;
}

void BMSearch(const unsigned char *pattern) {
	// create a buffer
	CBuffer stringToCompare;
	stringToCompare.size = strlen(pattern);
	stringToCompare.position = 0;
	// 1st step - build a shift table
	int shiftTable[S_TAB_LEN];
	BuildShiftTable(pattern, stringToCompare.size, shiftTable);
	// Fill a buffer from the text
	if (GetTextToString(stringToCompare.buffer, stringToCompare.size) == EOF) {
		return;
	}
	// 2nd step - main algorithm
	Search(pattern, &stringToCompare, shiftTable);
}

void GetString(unsigned char *str) {
	int c;
	int i = 0;
	while ((c = getc(stdin)) != '\n') {
		str[i] = c;
		++i;
	}
	str[i] = '\0';
}
