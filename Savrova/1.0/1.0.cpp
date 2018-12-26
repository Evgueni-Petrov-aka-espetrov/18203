#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int getstr(int shift, unsigned char* str);
void makeShift(int shift, unsigned char* str);
int search(int* shiftTable,unsigned char* pat, unsigned char* str, int* count);
void buildShiftTable( unsigned char* pat, int* shiftTable);
void BM( int* shiftTable, unsigned char* pat, unsigned char* str);

int main()
{
	unsigned char str[17] = {0};
	unsigned char pat[17];
	int shiftTable[256];
	scanf("%[^\n]s", pat);
	getchar();
	buildShiftTable(pat, shiftTable);
	BM(shiftTable, pat, str);
	return 0;
}

void buildShiftTable(unsigned char* pat, int* shiftTable)
{
	int pat_length = strlen((char*)pat);
	for (int i = 0; i < 256; i++)
		shiftTable[i] = pat_length;
	for (int i = 0; i < pat_length - 1; i++)
		shiftTable[pat[i]] = pat_length - 1 - i;
}

void BM(int* shiftTable, unsigned char* pat, unsigned char* str)
{   int y = 1;
	int  shift = 0;
	int count = 0;
	while (y)
	{   y = getstr(shift, str);
	    shift = search(shiftTable, pat, str, &count);
		makeShift(shift, str);
	}
}

int search(int* shiftTable, unsigned char* pat, unsigned char* str, int* count)
{
	int skip = 0;
	int str_length = strlen((char*)str);
	int pat_length = strlen((char*)pat);
	while (str_length - skip >= pat_length)
	{
		int i = pat_length;
		do
		{
			printf("%d ", *count + skip + i);
			i--;
		} while (str[skip + i] == pat[i] && i > 0);
		skip += shiftTable[str[skip + pat_length - 1]];
	}
	*count += skip;
	return str_length - skip;
}

void makeShift(int shift, unsigned char* str)
{
	for (int i = 0; i < shift; i++)
		str[i] = str[strlen(( char*)str) - shift + i];
}

int getstr(int shift, unsigned char* str)
{
	int count = fread(&str[shift], 1, 16 - shift, stdin);
	str[shift + count] = '\0';
	if (count == 16 - shift)
		return 1;
	return 0;
}
