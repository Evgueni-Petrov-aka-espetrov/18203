#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rabinkarp.h"
#define TRUE 1

int GetTextToString(unsigned char *str, int strLength, int startPos) {
	for (int i = startPos; i < strLength; ++i) {
		int c;
		if ((c = getc(stdin)) != EOF) {
			str[i] = c;
		}
		else {
			return EOF;
		}
	}
	return strLength - startPos; // return number of filled characters
}

int Power(int base, int p)
{
	int result = 1;
	while (p)
	{
		if (p & 1) result *= base;
		base *= base;
		p = p >> 1;
	}
	return result;
}

int PowerOfThree(int p) {
	const static int powerOfThree[16] = { 1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907 };
	if (p >= 0 && p <= 15) {
		return powerOfThree[p];
	} 
	else return Power(3, p);
}

int HashFuncChar(unsigned char ch, int number) {
	return (ch % 3) * PowerOfThree(number);
}

int HashFuncString(const unsigned char *string, int stringLength) {
	int sum = 0;
	for (int i = 0; i < stringLength; ++i) {
		sum += HashFuncChar(string[i], i);
	}
	return sum; 
}

int HashNext(int prevHash, unsigned char toDelete, unsigned char toAdd, int patLength) {
	return (prevHash - HashFuncChar(toDelete, 0)) / 3 + HashFuncChar(toAdd, patLength - 1);
}

int CompareStringsAndLog(const unsigned char *string, const unsigned char *pattern, int strLength, int posCount) {
	register int i = 0;
	for (; i < strLength; ++i) {
		fprintf(stdout, "%d ", posCount + 1 + i); // logging
		if (pattern[i] != string[(posCount + i) % strLength] /* i-th character of stringToHash */) {
			break;
		}
	}
	return i; // return strLength if strings are equal
}

int RKSearch(const unsigned char *pattern) {
	int patLength = strlen(pattern);
	int patHash = HashFuncString(pattern, patLength);
	fprintf(stdout, "%d ", patHash);
	int lastEntryPos = -1; // last pattern's entry in the text (-1 if the text doesn't contain the pattern)
	unsigned char stringToHash[MAX_PAT_LEN - 1]; // stringToHash will be a looped string
	if (GetTextToString(stringToHash, patLength, 0) == EOF) {
		return lastEntryPos;
	} 
	// calculating start hash value
	int currentHash = HashFuncString(stringToHash, patLength);
	int posCount = 0; // position in the text of the first character in stringToHash
	while (TRUE) {
		if (patHash == currentHash) {
			if (CompareStringsAndLog(stringToHash, pattern, patLength, posCount) == patLength) {
				lastEntryPos = posCount + 1;
			}
		}
		int nextChar;
		if ((nextChar = getc(stdin)) != EOF) {
			// calculating new hash value
			currentHash = HashNext(currentHash, stringToHash[posCount % patLength], // first character of stringToHash
												(unsigned char)nextChar, patLength);
			// add next character from the text to the end of stringToHash
			stringToHash[posCount % patLength] = (unsigned char)nextChar;
			// start from next character
			++posCount;
		}
		else break; // if EOF
	} 
	return lastEntryPos;
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