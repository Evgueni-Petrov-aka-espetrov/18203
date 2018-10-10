#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *SaveMalloc(size_t size) {
	void *ptr = NULL;
	if ((ptr = malloc(size)) == NULL) {
		fprintf(stderr, "not enough memory");
		exit(-1);
	}
	else return ptr; 
}

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

int power(int base, int p)
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

int HashFuncChar(unsigned char ch, int number) {
	return (ch % 3) * power(3, number);
}

int HashFuncString(unsigned char *string, int stringLength) {
	int sum = 0;
	for (int i = 0; i < stringLength; ++i) {
		sum += HashFuncChar(string[i], i);
	}
	return sum; 
}

int HashNext(int prevHash, unsigned char toDelete, unsigned char toAdd, int patLength) {
	return (prevHash - HashFuncChar(toDelete, 0)) / 3 + HashFuncChar(toAdd, patLength - 1);
}

void ShiftString(unsigned char *string, unsigned char ch, int strLength) {
	for (register int i = 1; i < strLength; ++i) {
		string[i - 1] = string[i];
	}
	string[strLength - 1] = ch;
}

int CompareStringsAndLog(unsigned char *firstString, unsigned char *secondString, int strLength, int posCount) {
	register int i = 0;
	for (; i < strLength; ++i) {
		fprintf(stdout, "%d ", posCount + i); // logging
		if (firstString[i] != secondString[i]) {
			break;
		}
	}
	return i;
}

int RKSearch(unsigned char *pattern) {
	int patLength = strlen(pattern);
	int patHash = HashFuncString(pattern, patLength);
	fprintf(stdout, "%d ", patHash);
	int lastEntryPos = -1;
	int posCount = 1;
	unsigned char *stringToHash = (unsigned char *)SaveMalloc(patLength);
	if (GetTextToString(stringToHash, patLength, 0) == EOF) {
		return lastEntryPos;
	}
	int currentHash = HashFuncString(stringToHash, patLength);
	if (currentHash == patHash) {
		if (CompareStringsAndLog(stringToHash, pattern, patLength, posCount) == patLength) {
			lastEntryPos = posCount;
		}
	}
	++posCount;
	int nextChar = '\0';
	while ((nextChar = getc(stdin)) != EOF) {
		currentHash = HashNext(currentHash, stringToHash[0], (unsigned char)nextChar, patLength);
		ShiftString(stringToHash, (unsigned char)nextChar, patLength);
		if (patHash == currentHash) {
			if (CompareStringsAndLog(stringToHash, pattern, patLength, posCount) == patLength) {
				lastEntryPos = posCount;
			}
		}
		++posCount;
	} 
	free(stringToHash);
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