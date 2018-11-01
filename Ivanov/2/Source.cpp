#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkInput(char* input);
int isDescending(char* str, int length);
void getNextPermutation(char* perm, int length);
void printPermutations(char* begin, int count);

int main() {
	char perm[11];
	int nCount;
	scanf("%s %d", perm, &nCount);
	if (!checkInput(perm)) {
		printf("bad input");
		return 0;
	}
	printPermutations(perm, nCount);
	return 0;
}

int isDescending(char* str, int length) {
	if (length < 2) return 0;
	for (int i = 0; i < length - 1; ++i)
		if (str[i] < str[i + 1]) return 0;
	return 1;
}

void getNextPermutation(char* perm, int length) {
	char cmpr = '9'; // [0..length] is not descending, [1..length] is descending
	for (int i = 1; i < length; ++i) // we search for the minimum element that more then [0], not-descending permutation cant begin with '9'
		if (perm[i] > perm[0] && perm[i] < cmpr) cmpr = perm[i];

	char* symbolSet = (char*)malloc(sizeof(char)*length+1);
	if (symbolSet == NULL) {
		printf("no memory");
		exit(1);
	}

	strcpy(symbolSet, perm);
	for (int i = 0; i < length; ++i) {
		if (symbolSet[i] == cmpr) {
			symbolSet[i] = ('9' + 1); // charcode is not a code of number
			break;
		}
	}
	perm[0] = cmpr;
	int writeIndex = 1;
	for (char i = '0'; i <= '9'; ++i) {
		for (int j = 0; j < length; ++j) {
			if (symbolSet[j] == i) {
				perm[writeIndex] = i;
				++writeIndex;
				break;
			}
		}
	}
	free(symbolSet);
}

void printPermutations(char* begin, int count) {
	int length = 0;
	for (; begin[length] != '\0'; ++length);
	if (length == 1) return;

	for (int i = length - 2; i >= 0; --i) {
		if (!isDescending(begin + i, length - i)) {
			if (--count < 0) return;
			getNextPermutation(begin + i, length - i);
			printf("%s\n", begin);
			i = length - 1;
		}
	}
}

int checkInput(char* input) {
	int count[10] = { 0 };
	for (int i = 0; input[i] != '\0'; ++i) {
		int symbol = input[i] - '0';
		if (symbol > 9 || symbol < 0) return 0;
		if (count[symbol]) return 0;
		++(count[symbol]);
	}
	return 1;
}