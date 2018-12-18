#include <iostream>
#define MAX_LEN_STR 16
#define MAX_LEN_PAT 16
#define MAX_LEN_TABLE 256

void buildShiftTable(unsigned char* pat, int* shiftTable);
void BM(int* shiftTable, unsigned char* pat, unsigned char* str);
int search(const int* shiftTable,unsigned char* pat, unsigned char* str, int* count);
void makeShift(int shift, unsigned char* str);
int getstr(int shift, unsigned char* str);
using namespace std;

int main() {
	unsigned char str[MAX_LEN_STR] = { 0 };
	unsigned char pat[MAX_LEN_PAT];
	int shiftTable[MAX_LEN_TABLE];
	cin >> pat;
	getchar();
	buildShiftTable(pat, shiftTable);
	BM(shiftTable, pat, str);
	return 0;
}

void buildShiftTable(unsigned char* pat, int* shiftTable)
{
	int pat_length = strlen((char*)pat);
	for (int i = 0; i < MAX_LEN_TABLE; i++)
		shiftTable[i] = pat_length;
	for (int i = 0; i < pat_length - 1; i++)
		shiftTable[pat[i]] = pat_length - 1 - i;
}

void BM(int* shiftTable, unsigned char* pat, unsigned char* str)
{
	int  shift(0);
	int count(0);
	while (getstr(shift, str)!=0)
	{
		shift = search(shiftTable, pat, str, &count);
		makeShift(shift, str);
	}
}

int search(const int* shiftTable,unsigned char* pat, unsigned char* str, int* count)
{
	int skip = 0;
	int str_length = strlen(( char*)str);
	int pat_length = strlen((char*)pat);
	while (str_length - skip >= pat_length)
	{
		int i = pat_length;
		while (str[skip + i] == pat[i] && i > 0)
		{
			cout << *count + skip + i <<" ";
			i--;
		}
		skip += shiftTable[str[skip + pat_length - 1]];
	}
	*count += skip;
	return str_length - skip;
}

void makeShift(int shift, unsigned char* str)
{
	for (int i = 0; i < shift; i++)
		str[i] = str[strlen((char*)str) - shift + i];
}

int getstr(int shift, unsigned char* str)
{
	int count = fread(&str[shift], 1, MAX_LEN_STR - shift, stdin);
	str[shift + count] = '\0';
	if (count == MAX_LEN_STR - shift)
		return 1;
	return 0;
}






