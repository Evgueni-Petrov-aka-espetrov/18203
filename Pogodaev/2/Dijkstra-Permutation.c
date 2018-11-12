#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void swap(char *a, char *b) {
	char t = *a;
	*a = *b;
	*b = t;
}

int IsInLexOrder(const char *string, int size) {
	int i;
	for (i = 0; i < size - 1; ++i) {
		if (string[i] < string[i + 1]) {
			break;
		}
	}
	if (i == size - 1) {
		return 1;
	}
	return 0;
}

void GeneratePerm(char *string, int size) {
	int i = size - 1;
	while (string[i] <= string[i - 1]) {
		--i;
	}
	int j = size;
	while (string[j - 1] <= string[i - 1]) {
		--j;
	}
	swap(&string[i - 1], &string[j - 1]);
	++i;
	j = size;
	while (i < j)
	{
		swap(&string[i - 1], &string[j - 1]);
		++i;
		--j;
	}
}

int GenNext(char *string, int size) {
	// return 0 if string is already in lexicographic order
	if (IsInLexOrder(string, size) == 1) {
		return 0;
	}
	// generating permutation
	GeneratePerm(string, size);
	return 1;
}

void Validation(const char *string, int size) {
	int checkArray[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < size; ++i) {
		if (!isdigit(string[i])) {
			fprintf(stdout, "bad input");
			exit(0);
		}
		if (checkArray[string[i] - '0'] != 0) {
			fprintf(stdout, "bad input");
			exit(0);
		}
		else {
			++checkArray[string[i] - '0'];
		}
	}
}

void Input(char *string, int *number) {
	if (fscanf(stdin, "%s", string) != 1) {
		fprintf(stdout, "bad input");
		exit(0);
	}
	if (fscanf(stdin, "%d", number) != 1) {
		fprintf(stdout, "bad input");
		exit(0);
	}
}

void Output(char *string, int size, int permCount) {
	for (int i = 0; i < permCount; ++i) {
		if (GenNext(string, size) != 0) {
			fprintf(stdout, "%s\n", string);
		}
		else {
			break;
		}
	}
}