#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX_LENGTH = 18;

int pow3(int grade){
	static int initialized = 0;
	static int gradeArray[MAX_LENGTH - 2];
	if (!initialized){
		int grade = 1;
		for (int i = 0; i < MAX_LENGTH - 2; ++i){
			gradeArray[i] = grade;
			grade *= 3;
		}
		initialized = 1;
	}
	return gradeArray[grade];
}

int hashFunc(char* str) {
	int result = 0;
	for (int i = 0; str[i] != '\0'; ++i) {
		result += (str[i] % 3)*pow3(i);
	}
	return result;
}

int shift(char* buffer, int length, int previousHash) {
	previousHash -= buffer[0] % 3;
	previousHash /= 3;
	strcpy(buffer, buffer + 1);
	int charcode;
	if ((charcode = getchar()) == EOF) return EOF;
	previousHash += (charcode % 3)*pow3(length - 1);
	buffer[length - 1] = charcode;
	buffer[length] = 0;
	int trueHash = hashFunc(buffer);
	return previousHash;
}

void compareStrings(char* pattern, char* buffer, int currentPosition) {
	for (int i = 0; pattern[i] != 0; ++i) {
		printf("%d ", currentPosition + i);
		if (pattern[i] != buffer[i]) break;
	}
}

void search(char* pattern) {
	int patternHash = hashFunc(pattern);
	printf("%d ", patternHash);
	int length = 0;
	for (; pattern[length] != 0; ++length);
	char* buffer = (char*)malloc(sizeof(char)*(length + 1));
	for (int i = 0; i < length; ++i) {
		int charcode;
		if ((charcode = getchar()) == EOF) {
			free(buffer);
			return;
		}
		buffer[i] = charcode;
	}
	buffer[length] = 0;

	int currentPosition = 1;
	int currentHash = hashFunc(buffer);
	for (;; ++currentPosition) {
		if (currentHash == patternHash) compareStrings(pattern, buffer, currentPosition);
		if ((currentHash = shift(buffer, length, currentHash)) == EOF) {
			free(buffer);
			return;
		}
	}
}

int main() {
	char pattern[MAX_LENGTH];
	gets_s(pattern);
	search(pattern);
	return 0;
}