#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 11
void Swap(char*, int, int);
int Permutation(char*, int);
void checkInput(char*);

int main()
{
	int permAmount;
	char Input[MAX_LENGTH];
	scanf("%s%d", Input, &permAmount);
	checkInput(Input);

	while (Permutation(Input, strlen(Input)) && permAmount--)
		printf("%s\n", Input);
	return 0;
}

void Swap(char* Input, int i, int j) {
	int t = Input[i];
	Input[i] = Input[j];
	Input[j] = t;
}

void checkInput(char* Input) {
	int a[MAX_LENGTH - 1] = { 0 };
	for (int i = 0; Input[i] != '\0'; ++i) {
		int num = Input[i] - '0';
		if (num > 9 || num < 0 || a[num]) {
			printf("bad input");
			exit;
		}
		++(a[num]);
	}
}

int Permutation(char* Input, int strlen) {
	int i = strlen - 1;
	for (;i > 0 && Input[i - 1] >= Input[i];i--)
		;
	if (!i)
		return 0;
	int j = strlen - 1;
	for (;Input[j] <= Input[i - 1];j--)
		;
	Swap(Input, i-1, j);
	j = strlen - 1;
	for (;i++ < j--; Swap(Input, i - 1, j))
		;
	return 1;
}
