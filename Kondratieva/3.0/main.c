#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN_STR 17
#define MAX_LEN_PAT 17
#define MAX_LEN_TABLE 256

void buildShiftTable(const unsigned char* pat, int* shiftTable) {
	int len = strlen((const char*)pat);
	for (int i = 0; i < MAX_LEN_TABLE; i++)
		shiftTable[i] = len;
	for (int i = 0; i < len - 1; i++)
		shiftTable[pat[i]] = len - 1 - i;
}

void makeShift(int shift, unsigned char* str) {
	for (int i = 0; i < shift; i++)
		str[i] = str[strlen((const char*)str) - shift + i];
}

int getString(int shift, unsigned char* str) {
	int count = fread(&str[shift], 1, (size_t)(MAX_LEN_STR - shift), stdin);
	str[shift + count] = '\0';
	if (count == MAX_LEN_STR - shift)
		return 1;
	return 0;
}

int Search(const int* shiftTable, const unsigned char* pat, const unsigned char* str) {
	int skip = 0, strLength = strlen((const char*)str), patLength = strlen((const char*)pat);
	static int count = 0;
	while (strLength - skip >= patLength) {
		int i = patLength;
		do {
			printf("%d ", count + skip + i);
			i--;
		} while (str[skip + i] == pat[i] && i > 0);
		skip += shiftTable[str[skip + patLength - 1]];
	}
	count += skip;
	return strLength - skip;
}

void boyerMoore(const int* shiftTable, unsigned char* pat, unsigned char* str) {
	int textIsOn = 1, shift = 0;
	while (textIsOn) {
		textIsOn = getString(shift, str);
		shift = Search(shiftTable, pat, str);
		makeShift(shift, str);
	}
}

int main() {
	unsigned char str[MAX_LEN_STR] = { 0 }, pat[MAX_LEN_PAT];
	int shiftTable[MAX_LEN_TABLE];
	scanf("%[^\n]s", pat);
	getchar();
	buildShiftTable(pat, shiftTable);
	boyerMoore(shiftTable, pat, str);
	return 0;
}
