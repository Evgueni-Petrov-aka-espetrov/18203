#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_PATTERN_LENGTH = 18;

int fillBuffer(char* buffer, int shift, int length, FILE* input);
void buildShiftTable(char* pattern, int* table, int length);
void search(char* pattern, FILE* input);
char* fgetsWithoutNewline(char* buffer, int maxCount, FILE* stream);

int main() {
	FILE* input = fopen("in.txt", "r");
	if (input == NULL) {
		printf("cannot open file\n");
		return 1;
	}
	char pattern[MAX_PATTERN_LENGTH];
	fgetsWithoutNewline(pattern, MAX_PATTERN_LENGTH, input);
	search(pattern, input);
	fclose(input);
	return 0;
}

int fillBuffer(char* buffer, int shift, int length, FILE* input) { ///length without terminating null
	strcpy(buffer, buffer + shift);
	if (fread(buffer + length - shift, sizeof(char), shift, input) != shift) {
		buffer[length] = '\0';
		return 0;
	}
	buffer[length] = '\0';
	return 1;
}

void buildShiftTable(char* pattern, int* table, int length) {
	for (int i = 0; i < 256; table[i++] = length);
	for (int i = 0; i < length - 1; ++i)
		table[pattern[i]] = length - i - 1;
}

void search(char* pattern, FILE* input) {
	int length = 0;
	for (; pattern[length] != '\0'; ++length); /// length without terminating null
	char* buffer = (char*)malloc(sizeof(char)*(length + 1));
	if (buffer == NULL) {
		printf("no memory");
		exit(1);
	}
	buffer[length] = '\0';
	if (!fillBuffer(buffer, length, length, input)) return;

	int shiftTable[256];
	buildShiftTable(pattern, shiftTable, length);

	int currentPosition = 1;
	for (;;) {
		for (int i = length - 1; i >= 0; --i) {
			printf("%d ", currentPosition + i);
			if (pattern[i] != buffer[i]) {
				int shift = i == length - 1 ? shiftTable[buffer[i]] : shiftTable[pattern[length - 1]];
				currentPosition += shift;
				if (!fillBuffer(buffer, shift, length, input)) {
					free(buffer);
					return;
				}
				break;
			}
			if (i == 0) {
				if (!fillBuffer(buffer, length, length, input)) {
					free(buffer);
					return;
				}
				currentPosition += length;
			}
		}
	}
}

char* fgetsWithoutNewline(char* buffer, int maxCount, FILE* stream) {
	char* result = fgets(buffer, maxCount, stream);
	for (int i = 0; buffer[i] != '\0'; ++i) {
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			break;
		}
	}
	return result;
}