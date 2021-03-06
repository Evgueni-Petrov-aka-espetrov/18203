#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

enum { STANDART_STRING_LENGTH = 16 };

void prepareStopTable(wchar_t* str, int strLength, int* table);
int readTemplate(wchar_t* str, FILE* file);
int badSymbol(wchar_t* str, int length, wchar_t* txt, int strEndPosition);
void read(FILE* file, int* str);
void write(int a);

int main() {
	int i = 0, length, badSymbolPosition, nextEndPosition, indexInStrs;
	int buf;
	int strbuf[STANDART_STRING_LENGTH + 1] = { 0 };
	wchar_t str[STANDART_STRING_LENGTH] = { 0 };
	wchar_t txt[STANDART_STRING_LENGTH] = { 0 };
	int stopTable[256]; //таблица последних позиций соотв. символов

	FILE* inputFile = fopen("in.txt", "r");

	if (inputFile != NULL) {
		length = readTemplate(str, inputFile);
		prepareStopTable(str, length, stopTable);

		nextEndPosition = i + length;
		read(inputFile, strbuf);
		buf = strbuf[0];
		while (buf != '\0') {
			indexInStrs = ++i;
			if (indexInStrs >= STANDART_STRING_LENGTH)
				indexInStrs %= STANDART_STRING_LENGTH;
			txt[indexInStrs] = buf;

			if (i == nextEndPosition) {
				nextEndPosition = nextEndPosition + length - 1 - stopTable[txt[indexInStrs]]; //определение след. позиции конца шаблона

				badSymbolPosition = badSymbol(str, length, txt, i);
			}

			if (indexInStrs == 0) read(inputFile, strbuf);
			buf = strbuf[indexInStrs];
		}
	}
	fclose(inputFile);
}

void read(FILE* file, int* str) {
	wchar_t* pAcceptedStr;

	for (int i = 0; i != STANDART_STRING_LENGTH; i++) {
		str[i] = fgetc(file);
		if (str[i] == EOF) {
			str[i] = '\0';
			break;
		}
	}
}

void write(int a) {
	printf_s("%d%s", a, " ");
}

int badSymbol(wchar_t* str, int length, wchar_t* txt, int strEndPosition) {
	bool isGoodPieceOfTxt = 1;
	int temp;
	for (int i = 0; i != length; i++) {
		write(strEndPosition - i); //вывод позиции
		temp = strEndPosition - i;
		if (temp >= STANDART_STRING_LENGTH)
			temp %= STANDART_STRING_LENGTH;
		if (str[length - 1 - i] != txt[temp]) {
			strEndPosition = temp;
			isGoodPieceOfTxt = 0;
			break;
		}
	}
	if (isGoodPieceOfTxt) strEndPosition = -1;
	return  strEndPosition;
}

int readTemplate(wchar_t* str, FILE* file) {
	int i = 0;
	wchar_t strbuf[STANDART_STRING_LENGTH + 2] = { 0 };
	fgetws(strbuf, STANDART_STRING_LENGTH + 2, file);
	str[i] = strbuf[i];
	while (strbuf[i] != '\n') {
		str[i] = strbuf[i++];
		//printf_s("%d", str[i]);
	}
	//fgetc(file); //избавление от '\n' (, идущего после '\0')
	return i;
}

void prepareStopTable(wchar_t* str, int strLength, int* table) {
	for (int i = 0; i != 256; i++) table[i] = -1;
	for (int i = 0; i != strLength - 1; i++) table[str[i]] = i;
}