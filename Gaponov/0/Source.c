#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define NUM_LENGTH 14
#define RES_LENGTH 100
int ConvToNum(char);
unsigned long long ToIntDec(char*, int);
double ToFracDec(char*, int);
double ToDec(char*, int);
char ConvToChar(int);
void FromDec(int b1, int b2, char *endNum, char *stNum,int);
void ReverseString(char *string, int);
void Swap(char *a, char *b);
int CheckInput(int, int, char*);


int main() {
	int b1, b2, i,flag;
	flag = 0;
	char stNum[NUM_LENGTH];
	char endNum[RES_LENGTH];

	for (i=0; i<RES_LENGTH;++i)
		endNum[i] = 0;


	scanf("%d%d%s", &b1, &b2, stNum);
	if (!(CheckInput(b1, b2, stNum))){
		printf("bad input");
		exit(0);
	}
	for (i=0; i < NUM_LENGTH; ++i)
		if (stNum[i] == '.')
			flag = 1;

	
	FromDec(b1, b2, endNum, stNum, flag);

	printf("%s %d", endNum, flag);
	//system("pause");
}

unsigned long long ToIntDec(char *n, int b1) {
	int i = 0;
	unsigned long long num=0;
	for (; n[i] != '.' && n[i] != '\0'; ++i)
		num = num*b1 + ConvToNum(n[i]);
	return num;
}

double ToFracDec(char *n, int b1) {
	long double num = 0, p = 1.0 / b1;
	int i;

	for (i = 0; n[i]!= '.'; ++i);

	for (i += 1; n[i] != '\0'; ++i) {
		num =num + p * ConvToNum(n[i]);
		p = p / b1;
	}
	return num;
}

double ToDec(char *n, int b1) {
	long double decNumber = 0.0;
	decNumber += ToIntDec(n, b1);
	decNumber += ToFracDec(n, b1);
	return decNumber;
}

int ConvToNum(char ch) {
	return (isdigit(ch) ? (ch - '0') : (ch - '7'));
}

char ConvToChar(int n) {
	if (n >= 0 && n <= 9)
		return n + 48;
	else if (n >= 10 && n <= 15)
		return n + 55;
	else {
		printf("bad input");
		exit(0);
	}
}

void Swap(char *a, char *b) {
	char t = *a;
	*a = *b;
	*b = t;
}

void ReverseString(char *string, int stringSize) {
	int i, j;
	for (i = stringSize - 1, j = 0; i > j; --i, ++j) {
		Swap(&string[i], &string[j]);
	}
}

int CheckInput(int b1, int b2, char *num) {
	int dotsCount = 0;
	for (int i = 0; num[i] != '\0'; ++i)
		num[i] = toupper(num[i]);
	if (b1 < 2 || b1>16 || b2 < 2 || b2>16)
		return 0;
	for (int i = 0; num[i] != '\0' ; ++i) {
		if (num[i] == '.') {
			++dotsCount;
			if (i == 0 || num[i + 1] == '\0' || dotsCount > 1)
				return 0;
		}
		else if (!(num[i] >= '0' && num[i] < ('0' + b1) || num[i] >= 'A' && num[i] < ('7' + b1) || num[i] == '.'))
			return 0;
	}
	return 1;
}

void FromDec(int b1, int b2, char *endNum, char *stNum, int flag)
{ 
	int i = 0;
	unsigned long long int intNum = ToIntDec(stNum, b1);

	if (intNum == 0) {
		endNum[0] = '0';
		++i;
	}
		


	for (; intNum; ++i)
	{
		endNum[i] = ConvToChar(intNum % b2);
		intNum = intNum / b2;
	}

	ReverseString(endNum, i);

	endNum[i] ='.';
	if (flag) {
	long double flNum = ToFracDec(stNum, b1);
	
		for (i += 1; i < RES_LENGTH; ++i)
		{
			flNum *= b2;
			endNum[i] = ConvToChar((int)flNum);
			flNum -= (int)flNum;
		}
		endNum[i - 1] = 0;
	}
	else  endNum[i + 1] = 0;
}