#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

enum { BUFFER_LENGTH = 16 };

int currentPosition = 0;

int deg(int base, int deg);
char readPattern(unsigned char* pattern, FILE* file);

struct TIStream {
	unsigned char buffer[BUFFER_LENGTH] = "";
	char bufferPointer = 0;
	unsigned char lastSymbol = '\0';
	bool readSymbol(FILE* file) {
		int buf = fgetc(file);
		if (buf == EOF)
			return 0;
		lastSymbol = buf;
		buffer[bufferPointer] = buf;
		if (++bufferPointer == BUFFER_LENGTH)
			bufferPointer = 0;
		return 1;
	}
	bool isBufferEqual(unsigned char* pattern, char patternLength) {
		char positionInBuffer = (BUFFER_LENGTH + bufferPointer - patternLength) % BUFFER_LENGTH;
		for (int i = 0; i != patternLength; ++i) {
			printf(" %d", currentPosition - patternLength + 1 + i);//для протокола
			if (positionInBuffer == BUFFER_LENGTH)
				positionInBuffer = 0;
			if (pattern[i] != buffer[positionInBuffer++])
				return 0;
		}
		return 1;
	}
};

struct THashCounter {
	int max3Degree;
	int currentHash;
	char stringLength;
	void prepare(char length) {
		stringLength = length;
		max3Degree = deg(3, length - 1);
		currentHash = 0;
	}
	int newHash(unsigned char newSymbol) {
		currentHash = currentHash / 3 + newSymbol % 3 * max3Degree;
		return currentHash;
	}
	int newHash(unsigned char* string) {
		for (int i = 0; i != stringLength; ++i)
			newHash(string[i]);
		return currentHash;
	}
};

int main() {
	FILE* inputFile = fopen("in.txt", "r");
	if (inputFile == NULL)
		return -1;
	TIStream inputStream;
	THashCounter hashCounter;
	unsigned char pattern[16 + 1];
	char  patternLength;
	int patternHash;

	patternLength = readPattern(pattern, inputFile);
	hashCounter.prepare(patternLength);
	patternHash = hashCounter.newHash(pattern);
	printf("%d", patternHash);

	hashCounter.prepare(patternLength);
	while (inputStream.readSymbol(inputFile)) {
		++currentPosition;
		if (hashCounter.newHash(inputStream.lastSymbol) == patternHash)
			if (inputStream.isBufferEqual(pattern, patternLength))
				;//здесь должна была быть радость от нахождения строки
	}

	fclose(inputFile);
}

char readPattern(unsigned char* pattern, FILE* file) {
	pattern[0] = fgetc(file);
	int i = 0;
	while (pattern[i] != '\n')
		pattern[++i] = fgetc(file);
	return i;
}

int deg(int base, int deg) {
	int res = 1;
	while (deg) {
		if (deg % 2)
			res *= base;
		deg = deg >> 1;
		base *= base;
	}
	return res;
}